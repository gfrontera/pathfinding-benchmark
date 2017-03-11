/*
 * Copyright 2015-2017 Guillermo Frontera <guillermo.frontera@upm.es>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "visibility_problem_solver.h"

#include <unordered_map>
#include <unordered_set>

#include <log/logger.h>
#include <exc/exception.h>
#include <geom/algorithms_2d.h>

#include "envelope.h"
#include "least_common_ancestor_calculator.h"

using namespace ues::pf::vg2d;

namespace
{
/** For each rank, contains the segment visible between ranks i and i+1. */
typedef std::vector< segment_index > visible_segment_vector;
/** Contains the segment indices to those segments that start or end at a given rank. */
typedef std::unordered_multimap< point_rank, segment_index > rank_to_segment;
}

const std::string component_name = "2D Visibility Graph Solver";


ues::math::numeric_type distance ( const point_index origin,
                        const segment_index target,
                        const ues::math::numeric_type angle,
                        const point_vector & points,
                        const segment_vector & segments )
{
    return ues::geom::point_to_segment_distance ( points[origin],
                                                  points[segments[target].first],
                                                  points[segments[target].second],
                                                  angle + ues::math::pi / 2,
                                                  2e-3 );
}


segment_index nearest_segment ( const point_index origin,
                                const segment_index segment1,
                                const segment_index segment2,
                                const point_vector & points,
                                const segment_vector & segments,
                                const angle_vector & angles,
                                const visibility_problem_solver::segment_rank_vector & segment_ranks )
{
    // If both segments are really the same, the result is trivial.
    if ( segment1 == segment2 )
    {
        return segment1;
    }
    if ( segment1 == NULL_SEGMENT_INDEX )
    {
        return segment2;
    }
    if ( segment2 == NULL_SEGMENT_INDEX )
    {
        return segment1;
    }

    visibility_problem_solver::segment_rank segment1_rank = segment_ranks[segment1];
    visibility_problem_solver::segment_rank segment2_rank = segment_ranks[segment2];

    point_rank min_rank = std::max ( segment1_rank.min, segment2_rank.min );
    point_rank max_rank = std::min ( segment1_rank.max, segment2_rank.max );

    if ( min_rank > max_rank )
    {
        throw ues::exc::exception ( "The two segments provided cannot be compared as they do not overlap", UES_CONTEXT );
    }

    ues::math::numeric_type dist_seg1 = distance ( origin, segment1, angles[min_rank], points, segments );
    ues::math::numeric_type dist_seg2 = distance ( origin, segment2, angles[min_rank], points, segments );

    if ( dist_seg1 + ues::math::epsilon < dist_seg2 )
    {
        return segment1;
    }
    else if ( dist_seg1 > dist_seg2 + ues::math::epsilon )
    {
        return segment2;
    }
    else // if ( std::abs( dist_seg1 - dist_seg2 ) < ues::math::epsilon )
    {
        if ( min_rank == max_rank )
        {
            // In this case, both segments are connected by one point, but do
            // not overlap in any other point. In this case, we return the closest
            // one, even if the difference is residual.
            if ( dist_seg1 < dist_seg2 )
            {
                return segment1;
            }
            else
            {
                return segment2;
            }
        }
        else
        {
            // If the segments are connected by one point and overlap at more than
            // that one point, then the nearest one must be found and returned.
            ues::math::numeric_type dist_seg1 = distance ( origin, segment1, angles[max_rank], points, segments );
            ues::math::numeric_type dist_seg2 = distance ( origin, segment2, angles[max_rank], points, segments );

            if ( dist_seg1 + ues::math::epsilon < dist_seg2 )
            {
                return segment1;
            }
            else if ( dist_seg1 > dist_seg2 + ues::math::epsilon )
            {
                return segment2;
            }
            else
            {
                throw ues::exc::exception ( "Cannot determine the nearest segment", UES_CONTEXT );
            }
        }
    }
}


rank_to_segment generate_rank_to_segment ( const segment_vector & segments,
                                           point_index origin,
                                           visibility_problem_solver::segment_rank_vector segment_ranks )
{
    rank_to_segment result;

    // Traverse all segments.
    for ( segment_vector::size_type i = 0; i < segments.size(); ++i )
    {
        // Do not include any segment that starts or ends at current point.
        if ( segments[i].first != origin && segments[i].second != origin )
        {
            result.insert ( { segment_ranks[i].min, i } );
            result.insert ( { segment_ranks[i].max, i } );
        }
    }
    return result;
}


visibility_problem_solver::visible_point_vector
compute_visible_points ( const point_vector & points,
                         const segment_vector & segments,
                         const point_index origin,
                         const point_index_vector & sorted_points,
                         const rank_vector & ranks,
                         const angle_vector & angles,
                         const visible_segment_vector & visible_segments )
{
    // Initialize the vector to all false.
    visibility_problem_solver::visible_point_vector visible_points ( points.size(), { false, NULL_SEGMENT_INDEX } );

    // Any point is visible from itself (right??).
    visible_points[origin].visible = true;

    // Get the coordinates of the origin point.
    const ues::geom::point<2> & origin_point = points[origin];

    point_index_vector::size_type i = 0;
    for ( point_rank rank = 0; rank < angles.size() - 1; ++rank )
    {
        // Compute distance to the nearest segment
        ues::math::numeric_type segment_distance = std::numeric_limits<ues::math::numeric_type>::max();
        segment_index segment = NULL_SEGMENT_INDEX;

        {
            // Distance to the visible segment before current point.
            const segment_index & prev_segment = visible_segments[ ( rank + visible_segments.size() - 1 ) % ( visible_segments.size() ) ];
            if ( prev_segment != NULL_SEGMENT_INDEX )
            {
                segment_distance = distance ( origin, prev_segment, angles[rank], points, segments );
                segment = prev_segment;
            }

            // If not the same, the distance to the visible segment after current point is computed,
            // and the minimum distance among the two is used.
            const segment_index & next_segment = visible_segments[rank];
            if ( next_segment != NULL_SEGMENT_INDEX && ( prev_segment == NULL_SEGMENT_INDEX || next_segment != prev_segment ) )
            {
                ues::math::numeric_type next_segment_distance = distance ( origin, next_segment, angles[rank], points, segments );
                if ( next_segment_distance < segment_distance )
                {
                    segment_distance = next_segment_distance;
                    segment = next_segment;
                }
            }
        }

        if ( i < ranks.size() && ranks[i] == rank )
        {
            // Find the nearest point(s) with current rank. Points with same coordinates may appear more than once,
            // thus the nearest point may not be unique.

            // First, assume the first point with current rank is the nearest one.
            std::unordered_set<point_index> nearest_points;
            ues::math::numeric_type nearest_point_distance;
            {
                point_index current_point = sorted_points[i];
                nearest_points.insert ( current_point );
                nearest_point_distance = origin_point.distance_to ( points[current_point] );

                // Also, if the segment is closer than current point, then set the occluding segment.
                if ( nearest_point_distance >= segment_distance )
                {
                    visible_points[ current_point ].segment = segment;
                }
                ++i;
            }

            // This loop updates the nearest point list so it really contains the nearest points.
            while ( i < ranks.size() && ranks[i] == rank )
            {
                // Get another point with current rank.
                point_index current_point = sorted_points[i];
                // Check if it is closer (or at least as close) to the origin as the current nearest points.
                ues::math::numeric_type current_point_distance = origin_point.distance_to ( points[current_point] );
                if ( nearest_point_distance > current_point_distance )
                {
                    // Found a closer point. Former nearest points are no longer valid.
                    nearest_points.clear();
                    nearest_points.insert ( current_point );
                    nearest_point_distance = current_point_distance;
                }
                else if ( nearest_point_distance == current_point_distance )
                {
                    // This point is as close as the nearest one. Add to the list.
                    nearest_points.insert ( current_point );
                }
                // If the segment is closer than current point, then set the occluding segment.
                if ( current_point_distance >= segment_distance )
                {
                    visible_points[ current_point ].segment = segment;
                }
                ++i;
            }

            if ( nearest_point_distance < segment_distance ||
                    ( segment != NULL_SEGMENT_INDEX && ( nearest_points.find ( segments[segment].first ) != nearest_points.end() ||
                                                         nearest_points.find ( segments[segment].second ) != nearest_points.end() ) ) )
            {
                for ( point_index nearest_point : nearest_points )
                {
                    visible_points[nearest_point].visible = true;
                }
            }
        }
    }

    return visible_points;
}


visible_segment_vector generate_envelope ( const point_vector & points,
                                           const segment_vector & segments,
                                           const angle_vector & angles,
                                           const point_index origin,
                                           const rank_to_segment & rts,
                                           const visibility_problem_solver::segment_rank_vector & segment_ranks,
                                           const rank_vector & actual_rank )
{
    // Initialize de visible segment vector.
    visible_segment_vector visible_segments ( angles.size() - 1, NULL_SEGMENT_INDEX );

    // Generate an empty envelope to use in the algorithm.
    envelope env ( angles.size() );

    // Traverse all the ranks.
    for ( point_rank cur_k = 0; cur_k < angles.size(); ++cur_k )
    {
        // Update the visible segment for previous rank.
        if ( cur_k > 0 && !env.empty() )
        {
            visible_segments[cur_k - 1] = env.head()->segment;
        }

        // Get all the segments that start or end at this rank.
        auto segs = rts.equal_range ( cur_k );
        for ( auto it = segs.first; it != segs.second; ++it )
        {
            segment_index current_segment = it->second;

            // Consider only segments with length greater than zero.
            if ( segment_ranks[current_segment].min < segment_ranks[current_segment].max )
            {
                // If this is the first point of the segment.
                if ( cur_k == segment_ranks[current_segment].min )
                {
                    // Create the new segment to be added.
                    envelope::segment_data seg_data_new = { current_segment, actual_rank[segment_ranks[current_segment].max] };
                    // Retrieve the shortest segment as long as the new one (if exists).
                    auto seg_data_longer = env.shortest_as_long_as ( seg_data_new );

                    // Determine the nearest segment.
                    segment_index nearest = seg_data_new.segment;
                    if ( seg_data_longer->segment != NULL_SEGMENT_INDEX )
                    {
                        nearest = nearest_segment ( origin, seg_data_new.segment, seg_data_longer->segment, points, segments, angles, segment_ranks );
                    }

                    // If no segment is longer than the new one, or the longer segment is farther, then
                    // the new segment is actually added to the envelope.
                    if ( nearest == seg_data_new.segment )
                    {
                        // Get an iterator to the position previous to where the new segment shall be inserted.
                        envelope::iterator seg_data_prev = env.pred ( seg_data_longer );

                        // If the segment at least as long as the new one is just as long, it can be deleted.
                        if ( seg_data_new.rank == seg_data_longer->rank )
                        {
                            env.erase ( seg_data_longer );
                        }
                        // Insert the new segment into the envelope.
                        env.insert ( seg_data_new, seg_data_prev );

                        // Delete shorter segments that are hidden behind the new one.
                        if ( seg_data_prev != env.end() )
                        {
                            nearest = nearest_segment ( origin, seg_data_new.segment, seg_data_prev->segment, points, segments, angles, segment_ranks );
                            while ( seg_data_prev != env.end() && nearest == seg_data_new.segment )
                            {
                                auto seg_data_aux = env.pred ( seg_data_prev );
                                env.erase ( seg_data_prev );
                                seg_data_prev = seg_data_aux;
                                if ( seg_data_aux != env.end() )
                                {
                                    nearest = nearest_segment ( origin, seg_data_new.segment, seg_data_prev->segment, points, segments, angles, segment_ranks );
                                }
                            }
                        }
                    }
                }
                else // if ( cur_k == segment_ranks[current_segment].max )
                {
                    // This is the second point of a segment.
                    // Delete the segment if it is still in the envelope.
                    // If it is in the envelope, it must be at the head.
                    if ( !env.empty() && env.head()->segment == current_segment )
                    {
                        env.erase ( env.head() );
                    }
                }
            }
        }
    }

    return visible_segments;
}


visible_segment_vector generate_left_envelope ( const point_vector & points,
                                                const segment_vector & segments,
                                                const angle_vector & angles,
                                                const point_index origin,
                                                const visibility_problem_solver::segment_rank_vector & segment_ranks,
                                                const least_common_ancestor_calculator & lca )
{
    rank_vector actual_rank ( angles.size() );
    for ( rank_vector::size_type i = 0; i < actual_rank.size(); ++i )
    {
        actual_rank[i] = i;
    }
    visibility_problem_solver::segment_rank_vector left_segment_ranks ( segment_ranks.size() );
    for ( visibility_problem_solver::segment_rank_vector::size_type i = 0; i < left_segment_ranks.size(); ++i )
    {
        left_segment_ranks[i].min = segment_ranks[i].min;
        left_segment_ranks[i].max = lca.compute ( segment_ranks[i].min + 1, segment_ranks[i].max + 1 ) - 1;
    }

    rank_to_segment rts = generate_rank_to_segment ( segments, origin, left_segment_ranks );

    return generate_envelope ( points, segments, angles, origin, rts, left_segment_ranks, actual_rank );
}


visible_segment_vector generate_right_envelope ( const point_vector & points,
                                                 const segment_vector & segments,
                                                 const angle_vector & angles,
                                                 const point_index origin,
                                                 const visibility_problem_solver::segment_rank_vector & segment_ranks,
                                                 const least_common_ancestor_calculator & lca )
{
    point_rank max_rank = angles.size() - 1;
    angle_vector reversed_angles ( angles.rbegin(), angles.rend() );
    rank_vector actual_rank ( angles.size() );
    for ( rank_vector::size_type i = 0; i < actual_rank.size(); ++i )
    {
        actual_rank[i] = max_rank - i;
    }
    visibility_problem_solver::segment_rank_vector right_segment_ranks ( segment_ranks.size() );
    for ( visibility_problem_solver::segment_rank_vector::size_type i = 0; i < right_segment_ranks.size(); ++i )
    {
        right_segment_ranks[i].min = max_rank - segment_ranks[i].max;
        right_segment_ranks[i].max = max_rank - ( lca.compute ( segment_ranks[i].min + 1, segment_ranks[i].max + 1 ) - 1 );
    }

    rank_to_segment rts = generate_rank_to_segment ( segments, origin, right_segment_ranks );

    visible_segment_vector visible_segments = generate_envelope ( points, segments, reversed_angles, origin, rts, right_segment_ranks, actual_rank );

    return visible_segment_vector ( visible_segments.rbegin(), visible_segments.rend() );
}


visible_segment_vector merge_visible_segments ( const point_vector & points,
                                                const segment_vector & segments,
                                                const angle_vector & angles,
                                                const point_index origin,
                                                const visible_segment_vector & visible_segments_left,
                                                const visible_segment_vector & visible_segments_right,
                                                const visibility_problem_solver::segment_rank_vector & segment_ranks )
{
    visible_segment_vector visible_segments ( visible_segments_left.size() );
    for ( visible_segment_vector::size_type i = 0; i < visible_segments.size(); ++i )
    {
        visible_segments[i] = nearest_segment ( origin, visible_segments_left[i], visible_segments_right[i], points, segments, angles, segment_ranks );
    }
    return visible_segments;
}


void visibility_problem_solver::solve_visibility ( const point_vector & points,
                                                   const segment_vector & segments,
                                                   const angle_vector & angles,
                                                   const point_index origin,
                                                   const point_index_vector & sorted_points,
                                                   const rank_vector & ranks,
                                                   const segment_rank_vector & segment_ranks,
                                                   visible_point_vector & visible_points )
{
    ues::log::logger lg;

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Starting envelope generation" );
        lg.record ( std::move ( e ) );
    }

    least_common_ancestor_calculator lca ( angles.size() );

    // Compute visible segments for each rank using left and right envelope.
    visible_segment_vector visible_segments_left = generate_left_envelope ( points, segments, angles, origin, segment_ranks, lca );
    visible_segment_vector visible_segments_right = generate_right_envelope ( points, segments, angles, origin, segment_ranks, lca );

    // Compute actual visible segments from the right and left visible segments.
    visible_segment_vector visible_segments = merge_visible_segments ( points, segments, angles, origin, visible_segments_left, visible_segments_right, segment_ranks );

    // Complete the point visibility info with the information computed in the previous stage.
    visible_points = compute_visible_points ( points, segments, origin, sorted_points, ranks, angles, visible_segments );

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Finished envelope generation" );
        e.message() << "Visible segments: ";
        std::for_each ( visible_segments.begin(), visible_segments.end(), [&e] ( segment_index s )
        {
            if ( s == NULL_SEGMENT_INDEX )
            {
                e.message() << "-, ";
            }
            else
            {
                e.message() << s << ", ";
            }
        } );
        e.message() << '\n' << "Visible points: ";
        std::for_each ( visible_points.begin(), visible_points.end(), [&e] ( const visibility_problem_solver::point_visibility_info & s ) { e.message() << s.visible << ", "; } );
        e.message() << '\n';
        lg.record ( std::move ( e ) );
    }

}
