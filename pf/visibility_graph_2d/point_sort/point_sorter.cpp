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

#include "point_sorter.h"

#include <list>

#include <log/logger.h>

#include "planar_graph_generator.h"

using namespace ues::pf::vg2d;

const std::string component_name = "Point Sorter";


point_sorter::point_sorter ( shared_point_vector points )
    : points ( std::move ( points ) ),
      graph ( planar_graph_generator::generate_planar_graph ( *this->points ) )
{
    ues::log::logger lg;
    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Generated planar graph" );
        e.message() << graph << '\n';
        lg.record ( std::move ( e ) );
    }
}


point_index_vector point_sorter::get_sorted_list_of_points ( point_index origin_point_index ) const
{
    ues::log::logger lg;

    // The origin_point corresponds with a line in the graph
    const ues::geom::point<2> origin_point = planar_graph_generator::transform ( graph.get_line ( origin_point_index ) );

    auto line_index_list = graph.get_sorted_lines ( origin_point_index );

    // See if the points corresponding to the line are to the right or left of the origin point
    // and add them to the corresponding list.
    std::list< point_index > right_side, left_side;

    for ( planar_graph::line line_index : line_index_list )
    {
        const ues::geom::point<2> other_point = planar_graph_generator::transform ( graph.get_line ( line_index ) );

        if ( origin_point.get_x() > other_point.get_x() )
        {
            right_side.push_back ( line_index );
        }
        else
        {
            left_side.push_back ( line_index );
        }
    }

    while ( !left_side.empty() && ( *points ) [left_side.front()].get_x() > ( *points ) [origin_point_index].get_x() )
    {
        point_index moved_point = left_side.front();
        left_side.pop_front();
        right_side.push_back ( moved_point );
    }

    // Merge both lists by appending the right side at the end of the left side.
    left_side.insert ( left_side.end(), right_side.begin(), right_side.end() );

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Finished sorting points" );
        e.message() << "Sorted point indices: ";
        std::for_each ( left_side.begin(), left_side.end(), [&e] ( const point_index p ) { e.message() << p << ", "; } );
        e.message() << '\n';
        lg.record ( std::move ( e ) );
    }

    return point_index_vector ( left_side.begin(), left_side.end() );
}
