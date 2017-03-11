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

#include "visibility_graph_generator.h"

#include <unordered_set>

#include <exc/exception.h>
#include <log/logger.h>

#include "envelope_generation/visibility_problem_solver.h"
#include "self_occlusion/polygon_self_occlusion.h"
#include "point_sort/point_sorter.h"
#include "util/util.h"

using namespace ues::pf::vg2d;

const std::string component_name = "2D Visibility Graph Generator";


/** Computes the angle between two points. The value returned is a number between 0 and 2*pi which
 * indicates the size, in radians, of a clockwise angle with one side being parallel to the positive
 * y-axis and the other side the segment that connects both points. */
ues::math::numeric_type compute_angle ( const ues::geom::point<2> & origin, const ues::geom::point<2> & end, const ues::math::numeric_type & epsilon )
{
    ues::math::numeric_type result = origin.angle_to ( end ) - ues::math::pi / 2;

    if ( result < -epsilon )
        result += 2 * ues::math::pi;

    return result;
}


/** Computes the rank of every point in the \a sorted_points vector. */
void compute_rank ( const point_vector & points,
                    const point_index_vector & sorted_points,
                    point_index origin,
                    rank_vector & ranks,
                    rank_vector & unsorted_ranks,
                    angle_vector & angles,
                    ues::log::logger & lg,
                    const ues::math::numeric_type & epsilon )
{
    const ues::geom::point<2> & origin_point = points.at ( origin );

    unsorted_ranks.resize ( points.size() );

    ues::math::numeric_type last_angle = 0;
    point_rank current_rank = 0;
    std::unordered_set<point_index> zero_points;

    angles.push_back ( last_angle );

    for ( point_index p : sorted_points )
    {
        ues::math::numeric_type current_angle = compute_angle ( origin_point, points.at ( p ), epsilon );

        if ( std::abs ( current_angle ) <= epsilon )
        {
            if ( last_angle > epsilon )
            {
                current_angle = 2 * ues::math::pi;
            }
            else
            {
                // Differenciate segment splitting points, as they appear
                // twice in the sorted_points list.
                if ( zero_points.find ( p ) != zero_points.end() )
                {
                    current_angle = 2 * ues::math::pi;
                }
                else
                {
                    zero_points.insert ( p );
                }
            }
        }

        if ( current_angle + epsilon < last_angle )
        {
            std::ostringstream error;
            error << "The points are not correctly sorted. Current angle is " << current_angle << " rad, previous angle was " << last_angle << " rad.\n";
            error << "Provided sorted points (from " << points [origin] << ") were:\n";
            for ( point_index ep : sorted_points )
            {
                error << ep << ". " << points[ep] << " [" << compute_angle ( origin_point, points[ep], epsilon ) << " rad]";
                if ( p == ep )
                {
                    error << " (current)";
                }
                error << '\n';
            }
            error << "Duplicated points may be a cause for incorrect sorting";
            throw ues::exc::exception ( error.str(), UES_CONTEXT );
        }

        if ( current_angle > last_angle + epsilon )
        {
            ranks.push_back ( ++current_rank );
            angles.push_back ( current_angle );
        }
        else
        {
            ranks.push_back ( current_rank );
        }
        last_angle = current_angle;
        if ( current_angle != 2 * ues::math::pi )
        {
            unsorted_ranks[p] = current_rank;
        }
    }

    if ( last_angle < 2 * ues::math::pi )
    {
        angles.push_back ( 2 * ues::math::pi );
    }

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Finished computing point ranks" );
        e.message() << "Ranks: ";
        std::for_each ( ranks.cbegin(), ranks.cend(), [&e] ( const point_rank pr ) { e.message() << pr << ", "; } );
        e.message() << '\n' << "Unsorted ranks: ";
        std::for_each ( unsorted_ranks.cbegin(), unsorted_ranks.cend(), [&e] ( const point_rank pr ) { e.message() << pr << ", "; } );
        e.message() << '\n';
        e.message() << '\n' << "Angles: ";
        std::for_each ( angles.cbegin(), angles.cend(), [&e] ( const ues::math::numeric_type & a ) { e.message() << a << ", "; } );
        e.message() << '\n';
        lg.record ( std::move ( e ) );
    }

}


/** Splits all the segments crossing the positive y-axis. */
void split_segments ( const scenario & current_scenario,
                      const point_index_vector & sorted_points,
                      const point_index origin,
                      scenario & fixed_scenario,
                      point_index_vector & fixed_sorted_points,
                      segment_index_vector & segment_equivalence,
                      ues::log::logger & lg )
{
    const ues::geom::point<2> & origin_point = current_scenario.get_points() [origin];

    point_vector fixed_points = current_scenario.get_points();
    segment_vector fixed_segments = current_scenario.get_segments();
    polygon_vector fixed_polygons;

    // Generate a map to be able to check if a point already exists.
    point_map added_points;
    for ( point_index i = 0; i < fixed_points.size(); ++i )
    {
        added_points.insert ( std::make_pair ( fixed_points[i], i ) );
    }

    segment_equivalence.resize ( fixed_segments.size() );
    for ( segment_index_vector::size_type i = 0; i < fixed_segments.size(); ++i )
    {
        segment_equivalence[i] = i;
    }

    for ( const polygon & p : current_scenario.get_polygons() )
    {
        polygon fixed_p;
        for ( const segment_index si : p )
        {
            // Add the segment to the fixed polygon.
            fixed_p.push_back ( si );

            // Check if the segment crosses the positive y-axis.
            const ues::geom::point<2> & p1 = current_scenario.get_points() [ fixed_segments[si].first ];
            const ues::geom::point<2> & p2 = current_scenario.get_points() [ fixed_segments[si].second ];

            if ( ( p1.get_x() <= origin_point.get_x() && origin_point.get_x() < p2.get_x() )
                    || ( p2.get_x() <= origin_point.get_x() && origin_point.get_x() < p1.get_x() ) )
            {
                ues::math::numeric_type dist1 = std::abs ( p1.get_x() - origin_point.get_x() );
                ues::math::numeric_type dist2 = std::abs ( p2.get_x() - origin_point.get_x() );

                ues::math::numeric_type intersection_y = p2.get_y() + ( p1.get_y() - p2.get_y() ) * dist2 / ( dist1 + dist2 );

                if ( intersection_y > origin_point.get_y() )
                {
                    // There is an intersection point.

                    // Add new point only if it hasn't been added before.
                    ues::geom::point<2> intersection_point ( origin_point.get_x(), intersection_y );
                    point_index intersection_point_index = add_if_not_present ( fixed_points, added_points, std::move ( intersection_point ) );

                    // Modify segments.
                    segment new_segment ( intersection_point_index, fixed_segments[si].second );
                    fixed_segments[si].second = intersection_point_index;
                    fixed_segments.push_back ( std::move ( new_segment ) );
                    segment_equivalence.push_back ( si );

                    // Add new segment to the polygon.
                    fixed_p.push_back ( fixed_segments.size() - 1 );

                }
            }
        }
        fixed_polygons.push_back ( std::move ( fixed_p ) );
    }

    // Add the new points at the start of the sorting
    for ( point_index_vector::size_type i = current_scenario.get_points().size(); i < fixed_points.size(); ++i )
    {
        fixed_sorted_points.push_back ( i );
    }
    // Add the rest of the ponts
    fixed_sorted_points.insert ( fixed_sorted_points.end(), sorted_points.begin(), sorted_points.end() );
    // Add the new points also at the end of the sorting
    for ( point_index_vector::size_type i = current_scenario.get_points().size(); i < fixed_points.size(); ++i )
    {
        fixed_sorted_points.push_back ( i );
    }

    fixed_scenario = scenario ( std::move ( fixed_points ), std::move ( fixed_segments ), std::move ( fixed_polygons ) );

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Finished splitting segments" );
        e.message() << "Points: ";
        std::for_each ( fixed_scenario.get_points().cbegin(), fixed_scenario.get_points().cend(), [&e] ( const ues::geom::point<2> & p ) { e.message() << p << ", "; } );
        e.message() << '\n' << "Segments: ";
        std::for_each ( fixed_scenario.get_segments().cbegin(), fixed_scenario.get_segments().cend(), [&e] ( const segment & se ) { e.message() << "( " << se.first << " - " << se.second << " ), "; } );
        e.message() << '\n' << "Polygons: ";
        std::for_each ( fixed_scenario.get_polygons().cbegin(), fixed_scenario.get_polygons().cend(), [&e] ( const polygon & po )
        {
            e.message() << "( ";
            std::for_each ( po.cbegin(), po.cend(), [&e] ( const segment_index & si ) { e.message() << si << " "; } );
            e.message() << "), ";
        } );
        e.message() << '\n' << "Sorted point indices: ";
        std::for_each ( fixed_sorted_points.begin(), fixed_sorted_points.end(), [&e] ( const point_index p ) { e.message() << p << ", "; } );
        e.message() << '\n';
        lg.record ( std::move ( e ) );
    }

}


visibility_problem_solver::segment_rank_vector
generate_segment_ranks ( const segment_vector & segments,
                         const rank_vector & unsorted_ranks,
                         const angle_vector & angles,
                         ues::log::logger & lg )
{
    const point_rank max_k = angles.size() - 1;

    visibility_problem_solver::segment_rank_vector rank_segments;
    for ( const segment & s : segments )
    {
        point_rank rank1 = unsorted_ranks[s.first];
        point_rank rank2 = unsorted_ranks[s.second];

        // Compute the rank of the points. Because points used to split
        // segments have two ranks, adjustments have to be made to the case where
        // the rank returned by the unsorted_ranks vector is incorrect.
        if ( rank1 == 0 && angles[rank2] > ues::math::pi )
        {
            rank1 = max_k;
        }
        else if ( rank2 == 0 && angles[rank1] > ues::math::pi )
        {
            rank2 = max_k;
        }

        rank_segments.push_back ( { std::min ( rank1, rank2 ), std::max ( rank1, rank2 ) } );
    }

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Finished computing segment ranks" );
        e.message() << "Segment ranks: ";
        std::for_each ( rank_segments.begin(), rank_segments.end(), [&e] ( const visibility_problem_solver::segment_rank & r ) { e.message() << "( " << r.min << " - " << r.max << " ), "; } );
        e.message() << '\n';
        lg.record ( std::move ( e ) );
    }

    return rank_segments;
}


/** Adds the information contained in the \a visible_segments and \a visible_points to the visibility graph */
void complete_visibility ( const point_vector & points,
                           const point_index origin,
                           const visibility_problem_solver::visible_point_vector & visible_points,
                           const segment_index_vector & segment_equivalence,
                           const polygon_self_occlusion & polygon_occlusion_info,
                           visibility_graph & graph )
{
    const ues::geom::point<2> & origin_point = points.at ( origin );

    for ( point_vector::size_type i = 0; i < points.size(); ++i )
    {
        const ues::geom::point<2> & destination_point = points.at ( i );
        segment_index self_occluding_segment_index;
        if ( polygon_occlusion_info.check_occluding_segment ( origin, i, self_occluding_segment_index ) )
        {
            graph.add_occlusion_segment ( origin_point, destination_point, self_occluding_segment_index );
        }
        else
        {
            if ( visible_points[i].visible )
            {
                graph.add_visibility ( origin_point, destination_point, origin_point.distance_to ( destination_point ) );
            }
            else
            {
                if ( visible_points[i].segment != NULL_SEGMENT_INDEX )
                {
                    graph.add_occlusion_segment ( origin_point, destination_point, segment_equivalence[ visible_points[i].segment ] );
                }
            }
        }
    }
}


visibility_graph_generator::visibility_graph_generator ( shared_point_vector points )
    : points ( std::move ( points ) ),
      sorter ( this->points ) //< Generate planar graph in O(n^2)
{
}


std::shared_ptr< visibility_graph >
visibility_graph_generator::generate_visibility_graph ( const shared_segment_vector & segments,
                                                        const shared_polygon_vector & polygons )
{
    ues::log::logger lg;

    try
    {
        if ( lg.min_level() <= ues::log::TRACE_LVL )
        {
            ues::log::event e ( ues::log::TRACE_LVL, component_name, "Start generating 2D visibility graph" );
            e.message() << "Points: ";
            std::for_each ( points->cbegin(), points->cend(), [&e] ( const ues::geom::point<2> & p ) { e.message() << p << ", "; } );
            e.message() << '\n' << "Segments: ";
            std::for_each ( segments->cbegin(), segments->cend(), [&e] ( const segment & se ) { e.message() << "( " << se.first << " - " << se.second << " ), "; } );
            e.message() << '\n' << "Polygons: ";
            std::for_each ( polygons->cbegin(), polygons->cend(), [&e] ( const polygon & po )
            {
                e.message() << "( ";
                std::for_each ( po.cbegin(), po.cend(), [&e] ( const segment_index & si ) { e.message() << si << " "; } );
                e.message() << "), ";
            } );
            e.message() << '\n';
            lg.record ( std::move ( e ) );
        }

        std::shared_ptr< visibility_graph > result = std::make_shared< visibility_graph > ( points, segments );

        // Build the scenario object.
        scenario current_scenario ( points, segments, polygons );

        // Generate self occluding polygon info.
        polygon_self_occlusion pso ( current_scenario );

        // For each point:
        for ( point_index point = 0; point < points->size(); ++point )
        {

            if ( lg.min_level() <= ues::log::TRACE_LVL )
            {
                ues::log::event e ( ues::log::TRACE_LVL, component_name, "Generating 2D visibility from point" );
                e.message() << "Point: " << point << " " << ( *points ) [point] << '\n';
                lg.record ( std::move ( e ) );
            }

            // Get a sorted list of all the other points.
            point_index_vector sorted_points = sorter.get_sorted_list_of_points ( point );

            // Split the segments that intersect the positive y-axis from origin point.
            scenario fixed_scenario;
            point_index_vector fixed_sorted_points;
            segment_index_vector segment_equivalence;
            split_segments ( current_scenario, sorted_points, point,
                             fixed_scenario, fixed_sorted_points, segment_equivalence, lg );

            // Compute rank of other points.
            rank_vector ranks;
            rank_vector unsorted_ranks;
            angle_vector angles;
            compute_rank ( fixed_scenario.get_points(), fixed_sorted_points, point, ranks, unsorted_ranks, angles, lg, ues::math::epsilon );

            visibility_problem_solver::segment_rank_vector segment_ranks = generate_segment_ranks ( fixed_scenario.get_segments(), unsorted_ranks, angles, lg );

            // Compute the visibility for current point.
            visibility_problem_solver::visible_point_vector visible_points;
            visibility_problem_solver::solve_visibility ( fixed_scenario.get_points(),
                                                          fixed_scenario.get_segments(),
                                                          angles,
                                                          point,
                                                          fixed_sorted_points,
                                                          ranks,
                                                          segment_ranks,
                                                          visible_points );

            // Fill the graph with envelope information.
            complete_visibility ( *points, point, visible_points, segment_equivalence, pso, *result );

        }

        if ( lg.min_level() <= ues::log::TRACE_LVL )
        {
            ues::log::event e ( ues::log::TRACE_LVL, component_name, "Finished generating 2D visibility graph" );
            e.message() << *result << '\n';
            lg.record ( std::move ( e ) );
        }

        return result;
    }
    catch ( ues::exc::exception & e )
    {
        throw ues::exc::exception ( std::move ( e ), UES_CONTEXT );
    }
}
