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

#include "rrt_pathfinder.h"

#include <exc/exception.h>
#include <log/logger.h>

#include "motion_planner.h"

using namespace ues::pf::mp;

const std::string component_name = "RRT Pathfinder";


ues::pf::path<3> rrt_pathfinder::find_path ( const ues::env::obstacle_vector & obstacles,
                                             const ues::geom::point<3> & origin,
                                             const ues::geom::point<3> & target ) const
{
    ues::log::logger lg;

    if ( lg.min_level() <= ues::log::DEBUG_LVL )
    {

        ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Computing RRT path" );
        e.message() << "From " << origin << " to " << target << ", with " << obstacles.size() << " obstacles:\n";
        e.message() << obstacles << '\n';
        lg.record ( std::move ( e ) );
    }

    try
    {
        motion_planner mp ( motion_planner::RRT );

        ues::pf::path<3> result = mp.plan_motion ( obstacles, origin, target );

        if ( lg.min_level() <= ues::log::DEBUG_LVL )
        {
            ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Found RRT path" );
            e.message() << result << '\n';
            lg.record ( std::move ( e ) );
        }

        return result;

    }
    catch ( ues::exc::exception & e )
    {
        throw ues::exc::exception ( std::move ( e ), "Error computing RRT path", UES_CONTEXT );
    }
}


rrt_pathfinder * rrt_pathfinder::clone() const &
{
    return new rrt_pathfinder ( *this );
}


rrt_pathfinder * rrt_pathfinder::clone() &&
{
    return new rrt_pathfinder ( std::move ( *this ) );
}

