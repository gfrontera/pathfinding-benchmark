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

#include <log/logger.h>
#include <geom/algorithms_2d.h>
#include <geom/algorithms_3d.h>

#include <pf/visibility_graph_2d/visibility_graph_generator.h>
#include <pf/visibility_graph_2d/envelope_generation/envelope.h>
#include <pf/visibility_graph_2d/util/scenario.h>
#include <pf/visibility_graph_2d/visibility_graph.h>
#include <pf/visibility_graph_2d/util/util.h>

using namespace ues::pf::vg3d;

/** Vector containing the height of each level. */
typedef std::vector< ues::math::numeric_type > obstacle_categories;
/** Vector containing the 2D visibility graph of each  level. */
typedef std::vector< std::shared_ptr< ues::pf::vg2d::visibility_graph > > visibilities;
/** Auxiliary type for some internal functions. */
typedef std::unordered_map< ues::geom::point<2>, ues::pf::vg2d::point_index > point_map;

const std::string component_name = "3D Visibility Graph Generator";

const unsigned int visibility_graph_generator::MAXIMUM_HEIGHTS = 10;


/** This method computes the point that must be traversed in order to reach point2 from point1 avoiding obstacles.
 * This method must be executed in constant time. */
void compute_slope ( const ues::geom::point<2> & point1,
                     const ues::geom::point<2> & point2,
                     const ues::pf::vg2d::point_vector & points,
                     const ues::pf::vg2d::segment_vector & segments,
                     const obstacle_categories & categories,
                     const ues::pf::vg2d::visibility_graph & current_level_data,
                     const obstacle_categories::size_type current_category,
                     ues::geom::point<3> & last_intersection,
                     bool & has_last_intersection,
                     visibility_graph & result )
{
    // Check what is the segment that stops visibility from point1 to point2
    ues::pf::vg2d::segment_index si;
    if ( current_level_data.check_occlusion_segment ( point1, point2, si ) )
    {
        // Compute the point where the segment from point1 to point2 intersects the occluding segment.
        ues::geom::point<2> intersection_2d = ues::geom::segment_intersection ( point1, point2, points[segments[si].first], points[segments[si].second], 10e-4 );
        ues::geom::point<2> last_intersection_2d = { last_intersection.get_x(), last_intersection.get_y() };

        if ( !has_last_intersection || intersection_2d != last_intersection_2d )
        {
            // If the intersection point is different than previous ones (a new obstacle is in the way), the point must be added.
            ues::geom::point<3> intersection_3d = { intersection_2d.get_x(), intersection_2d.get_y(), categories[current_category - 1] };

            // Check the point is not in the graph before adding it.
            if ( !result.has_point ( intersection_3d ) )
            {
                result.add_point ( intersection_3d );
            }
            // Connect the last intersection point with the new intersection point.
            if ( has_last_intersection )
            {
                result.add_visibility ( intersection_3d, last_intersection );
            }

            last_intersection = intersection_3d;
            has_last_intersection = true;

            for ( obstacle_categories::size_type i = 0; i < current_category; ++i )
            {
                // Connect higher points to the new intersection point.
                result.add_visibility ( ues::geom::point<3> ( point1.get_x(), point1.get_y(), categories[i] ), last_intersection );
            }
        }

        // Connect the current point to the new intersection point.
        result.add_visibility ( ues::geom::point<3> ( point1.get_x(), point1.get_y(), categories[current_category] ), last_intersection );
    }
}


std::shared_ptr< visibility_graph >
generate_visibility_graph ( const visibilities & level_data,
                            const ues::pf::vg2d::point_vector & points,
                            const ues::pf::vg2d::segment_vector & segments,
                            const obstacle_categories & categories,
                            const ues::geom::point<3> & origin,
                            const ues::geom::point<3> & target )
{
    // The output visibility graph.
    std::shared_ptr< visibility_graph > result ( new visibility_graph() );

    // Generate the initial set of points for the graph.
    for ( obstacle_categories::size_type i = 0; i < categories.size(); ++i )
    {
        for ( point_index pi = 0; pi < points.size(); ++pi )
        {
            result->add_point ( ues::geom::point<3> ( points[pi].get_x(), points[pi].get_y(), categories[i] ) );

            // Connect each point to the point immediately over it, as these points
            // are always connected due to the nature of the problem.
            if ( i > 0 )
            {
                result->add_visibility ( ues::geom::point<3> ( points[pi].get_x(), points[pi].get_y(), categories[i] ),
                                         ues::geom::point<3> ( points[pi].get_x(), points[pi].get_y(), categories[i - 1] ) );
            }
        }
    }

    // Add the origin and target points if not already in the graph.
    if ( !result->has_point ( origin ) )
    {
        result->add_point ( origin );
    }
    if ( !result->has_point ( target ) )
    {
        result->add_point ( target );
    }

    // Generate the connections between these points.
    for ( point_index pi1 = 1; pi1 < points.size(); ++pi1 )
    {
        const ues::geom::point<2> & point_2d_1 = points[pi1];

        for ( point_index pi2 = 0; pi2 < pi1; ++pi2 )
        {
            const ues::geom::point<2> & point_2d_2 = points[pi2];

            ues::geom::point<3> intersection_point1, intersection_point2;
            bool has_intersection_point1 = false, has_intersection_point2 = false;

            for ( obstacle_categories::size_type i = 0; i < categories.size(); ++i )
            {
                const ues::pf::vg2d::visibility_graph & current_level_data = *level_data[i];

                ues::math::numeric_type distance;
                if ( current_level_data.check_visibility ( point_2d_1, point_2d_2, distance ) )
                {
                    for ( obstacle_categories::size_type j = 0; j <= i; ++j )
                    {
                        // Make points visible
                        result->add_visibility ( ues::geom::point_2d_to_3d ( point_2d_1, categories[i] ),
                                                 ues::geom::point_2d_to_3d ( point_2d_2, categories[j] ) );
                        result->add_visibility ( ues::geom::point_2d_to_3d ( point_2d_1, categories[j] ),
                                                 ues::geom::point_2d_to_3d ( point_2d_2, categories[i] ) );
                    }
                }
                else
                {
                    bool connect_intersection_points = !has_intersection_point1 && !has_intersection_point2;

                    // Compute intersection points when going from point_2d_1 to point_2d_2.
                    compute_slope ( point_2d_1, point_2d_2, points, segments, categories, current_level_data,
                                    i, intersection_point1, has_intersection_point1, *result );

                    // Compute intersection points when going from point_2d_2 to point_2d_1.
                    compute_slope ( point_2d_2, point_2d_1, points, segments, categories, current_level_data,
                                    i, intersection_point2, has_intersection_point2, *result );

                    // Connect intersection points if necessary.
                    if ( connect_intersection_points && has_intersection_point1 && has_intersection_point2 )
                    {
                        result->add_visibility ( intersection_point1, intersection_point2 );
                    }

                }
            }
        }
    }

    return result;
}


visibilities compute_level_visibilities ( ues::pf::vg2d::scenario & current_scenario,
                                          const obstacle_categories & heights,
                                          const obstacle_categories & categories,
                                          ues::log::logger & lg )
{
    visibilities result;

    auto categories_it = categories.begin();
    auto polygons_it = current_scenario.get_polygons().begin();
    auto heights_it = heights.begin();
    ues::math::numeric_type current_category;

    // In every level, segments from zero to the value of this variable (excluded) are taken into account.
    ues::pf::vg2d::segment_index segments_until = 0;

    std::shared_ptr<ues::pf::vg2d::segment_vector> used_segments = std::make_shared<ues::pf::vg2d::segment_vector>();
    std::shared_ptr<ues::pf::vg2d::polygon_vector> used_polygons = std::make_shared<ues::pf::vg2d::polygon_vector>();

    ues::pf::vg2d::visibility_graph_generator graph_generator ( current_scenario.get_shared_points() );

    while ( categories_it != categories.end() )
    {
        current_category = *categories_it;

        if ( lg.min_level() <= ues::log::TRACE_LVL )
        {
            ues::log::event e ( ues::log::TRACE_LVL, component_name, "2D Visibility Graph for a horizontal cut of the environment" );
            e.message() << "Height of the cut: " << current_category << '\n';
            lg.record ( std::move ( e ) );
        }

        while ( heights_it != heights.end() && *heights_it > current_category )
        {
            // Current polygon is taken into account.
            used_polygons->push_back ( *polygons_it );

            // The segments conforming such polygon are taken into account, too.
            auto max_seg_it = std::max_element ( polygons_it->begin(), polygons_it->end() );
            if ( max_seg_it != polygons_it->end() && *max_seg_it >= segments_until )
            {
                for ( ues::pf::vg2d::segment_index i = segments_until; i <= *max_seg_it; ++i )
                {
                    used_segments->push_back ( current_scenario.get_segments() [i] );
                }
                segments_until = *max_seg_it + 1;
            }

            ++heights_it;
            ++polygons_it;

        }

        result.push_back ( graph_generator.generate_visibility_graph ( used_segments, used_polygons ) );

        categories_it++;
    }
    return result;
}


obstacle_categories compute_categories ( const obstacle_categories & heights,
                                         const ues::geom::point<3> & origin,
                                         const ues::geom::point<3> & target ) noexcept
{
    // Add all different heights as categories.
    obstacle_categories result;
    if ( heights.size() > visibility_graph_generator::MAXIMUM_HEIGHTS )
    {
        ues::math::numeric_type max_height = heights.front();
        for ( unsigned int i = visibility_graph_generator::MAXIMUM_HEIGHTS; i > 0; --i )
        {
            result.push_back ( max_height * i / visibility_graph_generator::MAXIMUM_HEIGHTS );
        }
    }
    else
    {
        for ( ues::math::numeric_type height : heights )
        {
            // Add a category for the building height.
            if ( result.empty() || result.back() > height )
            {
                result.push_back ( height );
            }
        }
    }

    // Add the zero-height category.
    result.push_back ( 0 );

    // Add the height of the origin point, if not already included.
    auto origin_it = std::lower_bound ( result.begin(), result.end(), origin.get_z(), [] ( ues::math::numeric_type a, ues::math::numeric_type b ) { return b < a; } );
    if ( origin_it != result.end() && *origin_it < origin.get_z() )
    {
        result.insert ( origin_it, origin.get_z() );
    }

    // Add the height of the target point, if not already included.
    auto target_it = std::lower_bound ( result.begin(), result.end(), target.get_z(), [] ( ues::math::numeric_type a, ues::math::numeric_type b ) { return b < a; } );
    if ( target_it != result.end() && *target_it < target.get_z() )
    {
        result.insert ( target_it, target.get_z() );
    }

    return result;
}


ues::env::obstacle_vector sort_obstacles ( ues::env::obstacle_vector obstacles, ues::log::logger & lg ) noexcept
{
    std::sort ( obstacles.begin(), obstacles.end(), [] ( const ues::env::obstacle & first, const ues::env::obstacle & second ) { return first.get_height() > second.get_height(); } );

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Sorted obstacles from higher to lower" );
        e.message() << obstacles << '\n';
        lg.record ( std::move ( e ) );
    }

    return obstacles;
}


void extract_obstacle_data ( const ues::env::obstacle_vector & obstacles,
                             const ues::geom::point<3> & origin,
                             const ues::geom::point<3> & target,
                             ues::pf::vg2d::scenario & current_scenario,
                             obstacle_categories & heights,
                             ues::log::logger & lg ) noexcept
{
    ues::env::obstacle_vector sorted_obstacles = sort_obstacles ( obstacles, lg );
    point_map added_points;

    ues::pf::vg2d::point_vector points;
    ues::pf::vg2d::segment_vector segments;
    ues::pf::vg2d::polygon_vector polygons;

    for ( const ues::env::obstacle & obs : sorted_obstacles )
    {
        ues::pf::vg2d::polygon current_polygon;
        ues::pf::vg2d::point_index first_point, prev_point;
        bool is_first = true;

        for ( const ues::geom::point<2> & p : obs.get_shape() )
        {
            // Position of the current point in the point vector.
            ues::pf::vg2d::point_index current_point;

            // Check whether the point is already in the point vector,
            // and initialize the current_point variable accordingly.
            current_point = ues::pf::vg2d::add_if_not_present ( points, added_points, p );

            if ( is_first )
            {
                first_point = current_point;
                is_first = false;
            }
            else
            {
                // Create a segment from previous to current points.
                ues::pf::vg2d::segment_index current_segment = segments.size();
                segments.push_back ( { prev_point, current_point } );
                current_polygon.push_back ( current_segment );
            }
            prev_point = current_point;
        }

        ues::pf::vg2d::segment_index current_segment = segments.size();
        segments.push_back ( { prev_point, first_point } );
        current_polygon.push_back ( current_segment );
        polygons.push_back ( current_polygon );
        heights.push_back ( obs.get_height() );
    }

    // Add origin and target to the point vector (if not in there already).
    ues::geom::point<2> origin_2d ( origin.get_x(), origin.get_y() );
    ues::pf::vg2d::add_if_not_present ( points, added_points, origin_2d );

    ues::geom::point<2> target_2d ( target.get_x(), target.get_y() );
    ues::pf::vg2d::add_if_not_present ( points, added_points, target_2d );

    current_scenario = ues::pf::vg2d::scenario ( std::move ( points ), std::move ( segments ), std::move ( polygons ) );
}


std::shared_ptr< visibility_graph >
visibility_graph_generator::generate_visibility_graph ( const ues::env::obstacle_vector & obstacles,
                                                        const ues::geom::point<3> & origin,
                                                        const ues::geom::point<3> & target )
{
    ues::pf::vg2d::scenario current_scenario;
    obstacle_categories heights;

    // Create a logger for the algorithm.
    ues::log::logger lg;

    extract_obstacle_data ( obstacles, origin, target, current_scenario, heights, lg );

    obstacle_categories categories = compute_categories ( heights, origin, target );

    // First, the two-dimensional pathfinding algorithm is executed for several level. The upper level is as
    // high as the highest building. Thus, all points are visible to each other at that level. In
    // lower levels, the obstacles that are higher than such level are taken into account.

    // Stores pathfinding information for each level.
    visibilities level_data = compute_level_visibilities ( current_scenario, heights, categories, lg );

    // Build 3D visibility graph from 2D level data.
    return ::generate_visibility_graph ( level_data, current_scenario.get_points(), current_scenario.get_segments(), categories, origin, target );
}
