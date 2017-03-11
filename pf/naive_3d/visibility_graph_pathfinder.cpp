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

#include <exc/exception.h>
#include <log/logger.h>

#include <pf/naive_3d/visibility_graph.h>
#include <pf/visibility_graph/graph_pathfinder.h>

using namespace ues::pf::naive_3d;

const std::string component_name = "Edge Sampling Pathfinder";


ues::pf::path<3> visibility_graph_pathfinder::find_path ( const ues::env::obstacle_vector & obstacles,
                                                          const ues::geom::point<3> & origin,
                                                          const ues::geom::point<3> & target ) const
{
    ues::log::logger lg;

    if ( lg.min_level() <= ues::log::DEBUG_LVL )
    {
        ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Computing edge sampling path" );
        e.message() << "From " << origin << " to " << target << " with " << obstacles.size() << " obstacles:\n";
        e.message() << obstacles << '\n';
        lg.record ( std::move ( e ) );
    }

    try
    {

        ues::pf::graph_pathfinder<3> finder ( std::make_shared<visibility_graph> ( obstacles, origin, target ) );
        ues::pf::path<3> result = finder.find_path ( origin, target );

        if ( lg.min_level() <= ues::log::DEBUG_LVL )
        {
            ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Found edge sampling path" );
            e.message() << result << '\n';
            lg.record ( std::move ( e ) );
        }

        return result;

    }
    catch ( ues::exc::exception & e )
    {
        throw ues::exc::exception ( std::move ( e ), "Error computing edge sampling path", UES_CONTEXT );
    }
}


visibility_graph_pathfinder * visibility_graph_pathfinder::clone() const &
{
    return new visibility_graph_pathfinder ( *this );
}


visibility_graph_pathfinder * visibility_graph_pathfinder::clone() &&
{
    return new visibility_graph_pathfinder ( std::move ( *this ) );
}
