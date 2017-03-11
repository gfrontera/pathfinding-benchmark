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

#include "visibility_graph_pathfinder.h"

#include <unordered_set>

#include <log/logger.h>

#include <pf/visibility_graph_2d/util/definitions.h>
#include <pf/visibility_graph_2d/util/util.h>
#include <pf/visibility_graph_2d/visibility_graph_generator.h>
#include <pf/visibility_graph/graph_pathfinder.h>

using namespace ues::pf::vg2d;

const std::string component_name = "2D Pathfinder";


ues::pf::path<2> visibility_graph_pathfinder::find_path ( const std::vector< ues::geom::polygon > & obstacles,
                                                          const ues::geom::point<2> & origin,
                                                          const ues::geom::point<2> & target )
{
    ues::log::logger lg;

    if ( lg.min_level() <= ues::log::DEBUG_LVL )
    {
        ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Start generating 2D path" );
        e.message() << "From " << origin << " to " << target << " with " << obstacles.size() << " obstacles:\n";
        std::for_each ( obstacles.begin(), obstacles.end(), [&e] ( const ues::geom::polygon & poly ) { e.message() << poly << '\n'; } );
        lg.record ( std::move ( e ) );
    }

    try
    {

        point_vector points;
        segment_vector segments;
        polygon_vector polygons;

        ues::pf::vg2d::point_map included_points;

        // Fill points, segments and polygons.
        for ( const ues::geom::polygon & poly : obstacles )
        {
            ues::pf::vg2d::polygon current_polygon;
            ues::pf::vg2d::point_index first_point, prev_point;
            bool is_first = true;

            for ( const ues::geom::point<2> & p : poly )
            {
                // Position of the current point in the point vector.
                ues::pf::vg2d::point_index current_point;

                // Check whether the point is already in the point vector,
                // and initialize the current_point variable accordingly.
                current_point = ues::pf::vg2d::add_if_not_present ( points, included_points, p );

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
        }

        ues::pf::vg2d::add_if_not_present ( points, included_points, origin );
        ues::pf::vg2d::add_if_not_present ( points, included_points, target );

        visibility_graph_generator vgg ( std::make_shared<point_vector> ( std::move ( points ) ) );
        ues::pf::graph_pathfinder<2> pf ( vgg.generate_visibility_graph ( std::make_shared<segment_vector> ( std::move ( segments ) ), std::make_shared<polygon_vector> ( std::move ( polygons ) ) ) );
        return pf.find_path ( origin, target );

    }
    catch ( ues::exc::exception & e )
    {
        throw ues::exc::exception ( std::move ( e ), "Error computing 2D path", UES_CONTEXT );
    }
}
