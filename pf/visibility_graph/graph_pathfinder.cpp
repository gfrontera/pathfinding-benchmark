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

#include "graph_pathfinder.h"

#include <unordered_set>
#include <unordered_map>

#include <boost/heap/fibonacci_heap.hpp>

#include <log/logger.h>
#include <exc/exception.h>

using namespace ues::pf;

const std::string component_name = "Pathfinder";

namespace
{

/** The state type contains a point index and the estimated cost of such point.*/
template<unsigned short N>
struct state
{
    typename visibility_graph<N>::size_type point_index;
    ues::math::numeric_type accumulated_cost;
    ues::math::numeric_type estimated_cost;
};

/** The state_comparator class contains the required comparator method so the
 * states are sorted in the desired order in the priority queue. */
template<unsigned short N>
struct state_comparator
{
    bool operator() ( const state<N> & one, const state<N> & two ) const noexcept
    {
        return one.estimated_cost > two.estimated_cost;
    }
};

/** The priority_queue stores and keeps a sorted list of state objects. */
template<unsigned short N>
using priority_queue = boost::heap::fibonacci_heap< state<N>, boost::heap::compare< state_comparator<N> > >;
/** The index_set contains all the points whose cost has already been computed. */
template<unsigned short N>
using index_set = std::unordered_set< typename visibility_graph<N>::size_type >;
/** Stores the handles that allow modifying the states of the priority queue. */
template<unsigned short N>
using handle_storage = std::unordered_map< typename visibility_graph<N>::size_type, typename priority_queue<N>::handle_type >;
/** Stores the point from which current point has been reached. */
template<unsigned short N>
using parent_point = std::vector< typename visibility_graph<N>::size_type >;

}


template<unsigned short N>
path<N> graph_pathfinder<N>::find_path ( const ues::geom::point<N> & origin,
                                         const ues::geom::point<N> & target )
{
    ues::log::logger lg;

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Starting pathfinding" );
        e.message() << *graph << '\n';
        lg.record ( std::move ( e ) );
    }

    // Get indices to the origin and target points.
    const typename visibility_graph<N>::size_type origin_index = graph->point_to_index ( origin );
    const typename visibility_graph<N>::size_type target_index = graph->point_to_index ( target );

    // Declare necesary data structures.
    priority_queue<N> frontier;
    handle_storage<N> handles;
    index_set<N> explored;
    parent_point<N> parents ( graph->size() );
    parents[ origin_index ] = origin_index;

    // Add initial node to the priority queue.
    typename priority_queue<N>::handle_type handle = frontier.push ( { origin_index, 0, origin.distance_to ( target ) } );
    handles.insert ( { origin_index, handle } );

    while ( !frontier.empty() )
    {
        state<N> node = frontier.top();
        frontier.pop();
        handles.erase ( node.point_index );

        if ( node.point_index == target_index )
        {
            path<N> reverse_result;
            typename visibility_graph<N>::size_type current_point = target_index;
            while ( current_point != parents[ current_point ] )
            {
                reverse_result.push_back ( graph->index_to_point ( current_point ) );
                current_point = parents[ current_point ];
            }
            reverse_result.push_back ( origin );
            path<N> result ( reverse_result.rbegin(), reverse_result.rend() );

            if ( lg.min_level() <= ues::log::DEBUG_LVL )
            {
                ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Found path" );
                e.message() << "Cost of the path: " << node.accumulated_cost << '\n';
                e.message() << result << '\n';
                lg.record ( std::move ( e ) );
            }

            return std::move ( result );
        }

        explored.insert ( node.point_index );

        for ( typename visibility_graph<N>::size_type p : graph->adjacents ( node.point_index ) )
        {
            if ( explored.find ( p ) == explored.end() )
            {
                // Get the cost of the edge from node.point_index to p.
                ues::math::numeric_type last_edge;
                graph->check_visibility ( node.point_index, p, last_edge );

                // Generate a node with the cost of getting to p from node.point_index.
                state<N> new_node;
                new_node.point_index = p;
                new_node.accumulated_cost = node.accumulated_cost + last_edge;
                ues::math::numeric_type heuristic_cost = graph->index_to_point ( p ).distance_to ( target );
                new_node.estimated_cost = new_node.accumulated_cost + heuristic_cost;

                typename handle_storage<N>::const_iterator it = handles.find ( p );
                if ( it == handles.end() )
                {
                    handle = frontier.push ( new_node );
                    handles.insert ( { p, handle } );
                    parents[p] = node.point_index;
                }
                else if ( new_node.estimated_cost < ( *it->second ).estimated_cost )
                {
                    ( *it->second ).estimated_cost = new_node.estimated_cost;
                    ( *it->second ).accumulated_cost = new_node.accumulated_cost;
                    frontier.decrease ( it->second );
                    parents[p] = node.point_index;
                }
            }
        }
    }

    throw ues::exc::exception ( "Unable to find a path between points", UES_CONTEXT );
}


template<unsigned short N>
graph_pathfinder<N>::graph_pathfinder ( std::shared_ptr< visibility_graph<N> > graph )
    : graph ( std::move ( graph ) )
{
    if ( this->graph.get() == nullptr )
        throw ues::exc::exception ( "Provided graph cannot be null", UES_CONTEXT );
}


// Instantiate the templates in this translation unit, just once.
template class ues::pf::graph_pathfinder<2>;
template class ues::pf::graph_pathfinder<3>;
