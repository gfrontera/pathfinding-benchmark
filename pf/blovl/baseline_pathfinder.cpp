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

#include "baseline_pathfinder.h"

#include <exc/exception.h>
#include <log/logger.h>

#include <pf/visibility_graph_3d/visibility_graph_pathfinder.h>
#include <pf/visibility_graph/graph_pathfinder.h>

using namespace ues::pf::blovl;

const std::string component_name = "Baseline Pathfinder";


ues::env::obstacle_vector filter_obstacles ( const ues::env::obstacle_vector & obstacles,
                                             const ues::geom::point<3> & origin,
                                             const ues::geom::point<3> & target ) noexcept
{
    ues::env::obstacle_vector filtered_obstacles;
    const ues::geom::segment<3> baseline ( origin, target );
    for ( ues::env::obstacle obs : obstacles )
    {
        ues::geom::point<3> intersection_point;
        if ( obs.check_intersection ( baseline, intersection_point ) )
        {
            filtered_obstacles.push_back ( obs );
        }
    }
    return filtered_obstacles;
}


bool baseline_path_clear ( const ues::env::obstacle_vector & obstacles,
                           const ues::geom::point<3> & origin,
                           const ues::geom::point<3> & target,
                           ues::geom::point<3> & next_point,
                           ues::log::logger & lg )
{
    assert ( origin != target );

    ues::env::obstacle_vector filtered_obstacles = filter_obstacles ( obstacles, origin, target );

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Computing baseline path" );
        e.message() << "From " << origin << " to " << target << " taking into account the following " << filtered_obstacles.size() << " obstacle(s):\n";
        e.message() << filtered_obstacles << '\n';
        lg.record ( std::move ( e ) );
    }

    if ( !filtered_obstacles.empty() )
    {
        ues::pf::vg3d::visibility_graph_pathfinder pathfinder;
        ues::pf::path<3> local_shortest_path = pathfinder.find_path ( filtered_obstacles, origin, target );
        assert ( local_shortest_path.size() > 1 );

        if ( local_shortest_path.size() > 2 && local_shortest_path[0].equal ( local_shortest_path[1] ) )
        {
            local_shortest_path.erase ( local_shortest_path.begin() );
        }
        if ( local_shortest_path.size() > 2 && local_shortest_path[local_shortest_path.size() - 1].equal ( local_shortest_path[local_shortest_path.size() - 2] ) )
        {
            local_shortest_path.pop_back();
        }

        next_point = std::move ( static_cast<ues::geom::point<3> &> ( local_shortest_path[1] ) );
        return local_shortest_path.size() == 2;
    }
    else
    {
        next_point = target;
        return true;
    }
}


ues::pf::path<3> baseline_pathfinder::find_path ( const ues::env::obstacle_vector & obstacles,
                                                  const ues::geom::point<3> & origin,
                                                  const ues::geom::point<3> & target ) const
{
    ues::log::logger lg;

    if ( lg.min_level() <= ues::log::DEBUG_LVL )
    {
        ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Computing approximate path with obstacle filtering" );
        e.message() << "From " << origin << " to " << target << ", with " << obstacles.size() << " obstacles:\n";
        e.message() << obstacles << '\n';
        lg.record ( std::move ( e ) );
    }

    try
    {

        ues::pf::path<3> result;
        result.push_back ( origin );

        ues::geom::point<3> partial_origin = origin;
        ues::geom::point<3> partial_target = target;

        while ( result.back() != target )
        {
            ues::geom::point<3> intermediate_point;
            if ( baseline_path_clear ( obstacles, partial_origin, partial_target, intermediate_point, lg ) )
            {
                result.push_back ( intermediate_point );
                partial_origin = std::move ( intermediate_point );
                partial_target = target;
            }
            else
            {
                partial_target = std::move ( intermediate_point );
            }
        }

        if ( lg.min_level() <= ues::log::DEBUG_LVL )
        {
            ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Found approximate path with obstacle filtering" );
            e.message() << result << '\n';
            lg.record ( std::move ( e ) );
        }

        return result;

    }
    catch ( ues::exc::exception & e )
    {
        throw ues::exc::exception ( std::move ( e ), "Error computing approximate path with obstacle filtering", UES_CONTEXT );
    }
}


baseline_pathfinder * baseline_pathfinder::clone() const &
{
    return new baseline_pathfinder ( *this );
}


baseline_pathfinder * baseline_pathfinder::clone() &&
{
    return new baseline_pathfinder ( std::move ( *this ) );
}
