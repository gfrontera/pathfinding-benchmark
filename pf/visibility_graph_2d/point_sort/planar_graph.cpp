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

#include "planar_graph.h"

#include <cassert>
#include <unordered_set>

using namespace ues::pf::vg2d;


planar_graph::planar_graph ( ues::math::numeric_type epsilon ) noexcept
    : epsilon ( std::move ( epsilon ) )
{
    // Insert special vertex. This is the vertex that all edges
    // that go to infinity intersect.
    vertices.push_back ( { ues::geom::point<2>(), 0 } );
}


void planar_graph::add_line ( line_2d new_line )
{
    line line_index = lines.size();
    lines.push_back ( { std::move ( new_line ) } );

    // Check whether the graph is empty
    if ( edges.size() == 0 )
    {
        // The graph is empty, so the first edge is added.
        edge edge_index = edges.size();

        edge_info ei;
        ei.tail_vertex = 0;
        ei.head_vertex = 0;
        ei.prev_edge = edge_index;
        ei.next_edge = edge_index;
        ei.line_index = line_index;

        edges.push_back ( ei );
    }
    else
    {
        // The graph is non-empty, so it must be checked where the new line
        // intersects with previous edges.

        vertex intersection_index = 0; // Initialized to supress a warning.

        // Check intersection with edges corresponding to the first line
        std::vector< edge > first_line_edges = get_edges_of_line ( 0 );
        bool found = false;
        for ( auto it = first_line_edges.begin(); !found && it != first_line_edges.end(); ++it )
        {
            ues::geom::point<2> new_vertex;
            edge edge_index = *it;
            if ( edge_line_intersection ( edge_index, line_index, new_vertex ) )
            {
                intersection_index = split_edge ( edge_index, std::move ( new_vertex ) );
                found = true;
            }
        }

        // Makes sure intersection_index has been found
        assert ( found );

        edge header_index = vertices[intersection_index].header_index;
        edge current_edge_index = header_index;
        do
        {
            const edge_info & current_edge = edges[current_edge_index];

            if ( current_edge.line_index == 0 )
            {
                follow_line ( current_edge_index, intersection_index, line_index );
            }

            current_edge_index = get_next_edge ( current_edge_index, intersection_index );
        }
        while ( current_edge_index != header_index );

    }
}


void planar_graph::follow_line ( planar_graph::edge edge_index, planar_graph::vertex vertex_index, planar_graph::line line_index )
{
    // First, compute toward what direction we expecto to find next intersections. If the
    // initial edge goes left from the initial vertex, then we are going to find new intersections
    // towards +infinity. Otherwise, towards -infinity.
    bool is_up;
    if ( ( edges[edge_index].head_vertex == vertex_index && lines[line_index].geometric_info.get_x1() > lines[0].geometric_info.get_x1() )
            || ( edges[edge_index].tail_vertex == vertex_index && lines[line_index].geometric_info.get_x1() < lines[0].geometric_info.get_x1() ) )
    {
        is_up = true;
    }
    else
    {
        is_up = false;
    }

    // This algorithm may hang due to numeric precision issues. In order to avoid an infinite loop, a maximum number of iterations is set.
    const unsigned int MAX_ITERATIONS = edges.size() * 2;
    unsigned int num_iterations = 0;

    vertex last_line_vertex = vertex_index;
    do
    {
        ues::geom::point<2> intersection;
        vertex intersection_index;

        assert ( edges[edge_index].head_vertex == vertex_index || edges[edge_index].tail_vertex == vertex_index );

        bool intersection_found = false;

        if ( edge_line_intersection ( edge_index, line_index, intersection ) )
        {
            // If there is an intersection between current edge and the new line, split current segment
            // in two, inserting a new vertex at the intersection point.
            intersection_index = split_edge ( edge_index, std::move ( intersection ) );

            // Check that the intersection vertex isn't the start of current edge
            if ( intersection_index != vertex_index )
            {
                // A new vertex has been added to the graph
                intersection_found = true;
            }

            // Current edge has been splitted. Must determine which half corresponds to the
            // edge in contact with current vertex.
            if ( edges[edge_index].head_vertex != vertex_index && edges[edge_index].tail_vertex != vertex_index )
            {
                edge_index = get_next_edge ( edge_index, intersection_index );
            }
        }

        if ( intersection_found )
        {
            // Adding a new edge joining the last two intersection points.
            // The order of the vertices must be always be from left to right.
            if ( is_up )
                add_edge ( last_line_vertex, intersection_index, line_index );
            else
                add_edge ( intersection_index, last_line_vertex, line_index );

            // Save last intersection point
            last_line_vertex = intersection_index;

            // Proceed to next edge
            edge_index = get_next_edge ( edge_index, vertex_index );
        }
        else
        {
            // If there is no intersection, simply proceed to the next edge within the
            // same face.
            const edge_info & current_edge = edges[edge_index];
            if ( current_edge.head_vertex == vertex_index )
            {
                edge_index = get_next_edge ( edge_index, current_edge.tail_vertex );
                vertex_index = current_edge.tail_vertex;
            }
            else
            {
                edge_index = get_next_edge ( edge_index, current_edge.head_vertex );
                vertex_index = current_edge.head_vertex;
            }
        }

        if ( num_iterations > MAX_ITERATIONS )
        {
            throw ues::exc::exception ( "Planar graph generation exceeded maximum iteration count", UES_CONTEXT );
        }
        num_iterations++;

    }
    while ( vertex_index != 0 || is_up == upper_side ( edge_index, vertex_index, line_index ) );

    // The order of the vertices must be always be from left to right
    if ( is_up )
        add_edge ( last_line_vertex, 0, line_index );
    else
        add_edge ( 0, last_line_vertex, line_index );
}


bool planar_graph::upper_side ( planar_graph::edge edge_index, planar_graph::vertex vertex_index, planar_graph::line line_index ) const
{
    assert ( edges[edge_index].tail_vertex == vertex_index || edges[edge_index].head_vertex == vertex_index );

    const line_2d & geom_line = lines[line_index].geometric_info;

    if ( vertex_index == 0 )
    {
        // Vertex 0 is a special case, we need to check what is the gradient of the line it belongs to
        // to determine if the point is on the upper side of the reference line.
        const line_2d & edge_line = lines[edges[edge_index].line_index].geometric_info;
        if ( edges[edge_index].tail_vertex == 0 )
        {
            return edge_line.get_x1() < geom_line.get_x1();
        }
        else
        {
            return edge_line.get_x1() > geom_line.get_x1();
        }
    }
    else
    {
        // Simple geometric calculation for all other cases.
        const ues::geom::point<2> & geom_vertex = vertices[vertex_index].geometric_info;
        return geom_line.evaluate ( geom_vertex.get_x() ) > geom_vertex.get_y();
    }
}


bool planar_graph::edge_line_intersection ( planar_graph::edge edge_index, planar_graph::line line_index, ues::geom::point<2> & result ) const
{
    const edge_info & ei = edges[edge_index];
    const line_2d & first_line = lines[ei.line_index].geometric_info;
    const line_2d & second_line = lines[line_index].geometric_info;

    if ( first_line.get_x1() == second_line.get_x1() )
    {
        // Both lines have the same slope (gradient).
        if ( first_line.get_x0() == second_line.get_x0() )
        {
            // Both lines are actually the same! This case is not
            // supported by this method.
            throw ues::exc::exception ( "The edge is contained in the line and no intersection point can be computed", UES_CONTEXT );
        }
        else
        {
            // The edge is parallel to the line, so there is no intersection.
            throw ues::exc::exception ( "Parallel lines are not supported", UES_CONTEXT );
        }
    }
    else
    {
        // Lines have a different slope (gradient).
        ues::math::numeric_type x, y;
        x = - ( first_line.get_x0() - second_line.get_x0() ) / ( first_line.get_x1() - second_line.get_x1() );
        y = first_line.get_x1() * x + first_line.get_x0();
        result = { x, y };
        if ( ( ei.tail_vertex == 0 || x >= vertices[ei.tail_vertex].geometric_info.get_x() ) &&
                ( ei.head_vertex == 0 || x <= vertices[ei.head_vertex].geometric_info.get_x() ) )
        {
            return true;
        }
        else
        {
            // The checks below avoid some numeric errors.
            if ( ( ei.head_vertex > 0 && result.distance_to ( vertices[ei.head_vertex].geometric_info ) < epsilon ) ||
                    ( ei.tail_vertex > 0 && result.distance_to ( vertices[ei.tail_vertex].geometric_info ) < epsilon ) )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}


/** Splits an edge of the graph by a vertex. If the vertex matches one of the limits of the
 * edge, then no splitting is done. Otherwise, the new vertex is added to the graph. The index
 * of the vertex matching the coordinates given by the new vertex is returned. */
planar_graph::vertex planar_graph::split_edge ( planar_graph::edge edge_index, ues::geom::point<2> new_vertex )
{
    // Only one edge is created. The other part of the splitted edge uses the
    // previously existing edge.

    // Retrieving edge info
    edge_info & ei = edges[edge_index];

    if ( ei.head_vertex > 0 && new_vertex.distance_to ( vertices[ei.head_vertex].geometric_info ) < epsilon )
    {
        // No need to add any vertex
        return ei.head_vertex;
    }
    else if ( ei.tail_vertex > 0 && new_vertex.distance_to ( vertices[ei.tail_vertex].geometric_info ) < epsilon )
    {
        // No need to add any vertex
        return ei.tail_vertex;
    }
    else
    {
        // Index for the new vertex
        vertex vertex_index = vertices.size();
        // Index for the new edge
        edge new_edge_index = edges.size();

        // Create new edge
        edge_info new_ei ( ei );
        new_ei.tail_vertex = vertex_index;
        new_ei.prev_edge = edge_index;

        // Update existing edge
        ei.head_vertex = vertex_index;
        ei.next_edge = new_edge_index;

        // Add new edge
        edges.push_back ( std::move ( new_ei ) );

        // Update edge at the other end
        update_edge_references ( new_edge_index, edge_index );

        // Insert new vertex
        vertices.push_back ( { std::move ( new_vertex ), new_edge_index } );

        return vertex_index;
    }

}


void planar_graph::add_edge ( planar_graph::vertex tail_vertex_index, planar_graph::vertex head_vertex_index, planar_graph::line line_index )
{
    // If tail and head vertices are the same, there is no need to add any edge.
    // Proceed only if both are different.
    if ( tail_vertex_index != head_vertex_index )
    {

        // Generate info for the new edge
        edge new_edge_index = edges.size();
        edges.push_back ( edge_info() );
        edge_info & ei = edges[new_edge_index];
        ei.tail_vertex = tail_vertex_index;
        ei.head_vertex = head_vertex_index;
        ei.line_index = line_index;

        update_edges ( new_edge_index, tail_vertex_index );
        update_edges ( new_edge_index, head_vertex_index );

    }
}


void planar_graph::update_edges ( planar_graph::edge new_edge_index, planar_graph::vertex vertex_index )
{

    edge first_edge_index = vertices[vertex_index].header_index;
    edge current_edge_index = first_edge_index;
    edge next_edge_index;

    edge_info * current_edge = &edges[current_edge_index];
    edge_info * next_edge;

    edge_info & new_edge = edges[new_edge_index];

    ues::math::numeric_type current_edge_alpha = compute_alpha ( current_edge_index, vertex_index );
    ues::math::numeric_type next_edge_alpha;

    ues::math::numeric_type new_edge_alpha = compute_alpha ( new_edge_index, vertex_index );

    do
    {
        // Retrieve information from next edge
        next_edge_index = get_next_edge ( current_edge_index, vertex_index );
        next_edge = &edges[next_edge_index];
        next_edge_alpha = compute_alpha ( next_edge_index, vertex_index );

        if ( ( current_edge_alpha <= next_edge_alpha && ( current_edge_alpha < new_edge_alpha && new_edge_alpha <= next_edge_alpha ) )
                || ( current_edge_alpha > next_edge_alpha && ( current_edge_alpha < new_edge_alpha || new_edge_alpha <= next_edge_alpha ) ) )
        {
            // Edges must be updated to include the new edge
            if ( new_edge.tail_vertex == vertex_index )
            {
                new_edge.prev_edge = next_edge_index;
            }
            else
            {
                new_edge.next_edge = next_edge_index;
            }

            if ( current_edge->tail_vertex == vertex_index )
            {
                current_edge->prev_edge = new_edge_index;
            }
            else
            {
                current_edge->next_edge = new_edge_index;
            }
            return;
        }

        // Copy next edge information to current edge
        current_edge_index = next_edge_index;
        current_edge = next_edge;
        current_edge_alpha = next_edge_alpha;
    }
    while ( next_edge_index != first_edge_index );
    throw ues::exc::exception ( "Failed to add new edge to a vertex", UES_CONTEXT );
}


planar_graph::edge planar_graph::get_next_edge ( planar_graph::edge edge_index, planar_graph::vertex vertex_index ) const
{
    const edge_info & ei = edges[edge_index];

    assert ( ei.head_vertex == vertex_index || ei.tail_vertex == vertex_index );

    if ( ei.head_vertex == vertex_index )
        return ei.next_edge;
    else
        return ei.prev_edge;

}


ues::math::numeric_type planar_graph::compute_alpha ( planar_graph::edge edge_index, planar_graph::vertex vertex_index ) const
{
    const edge_info & ei = edges[edge_index];

    assert ( ei.head_vertex == vertex_index || ei.tail_vertex == vertex_index );

    ues::math::numeric_type result;

    if ( vertex_index > 0 )
    {
        if ( ei.tail_vertex == vertex_index )
        {
            result = std::atan ( lines[ei.line_index].geometric_info.get_x1() );
        }
        else
        {
            result = std::atan ( lines[ei.line_index].geometric_info.get_x1() );
            if ( result < 0 )
            {
                result += ues::math::pi;
            }
            else
            {
                result -= ues::math::pi;
            }
        }
    }
    else
    {
        if ( ei.tail_vertex == vertex_index )
        {
            result = ues::math::pi / 2 - std::atan ( lines[ei.line_index].geometric_info.get_x1() );
        }
        else
        {
            result = -ues::math::pi / 2 - std::atan ( lines[ei.line_index].geometric_info.get_x1() );
        }
    }
    return result;
}


std::vector< planar_graph::edge > planar_graph::get_edges_of_line ( planar_graph::line line_index ) const
{
    std::vector< edge > result;

    assert ( line_index < lines.size() );

    vertex vertex_index = 0;
    do
    {
        edge edge_index = vertices.at ( vertex_index ).header_index;
        while ( edges.at ( edge_index ).line_index != line_index ||
                edges.at ( edge_index ).tail_vertex != vertex_index )
        {
            edge_index = get_next_edge ( edge_index, vertex_index );
        }
        result.push_back ( edge_index );
        vertex_index = edges.at ( edge_index ).head_vertex;
    }
    while ( vertex_index != 0 );

    return result;
}


std::vector< planar_graph::line > planar_graph::get_sorted_lines ( planar_graph::line line_index ) const
{
    std::vector< line > result;
    std::unordered_set< line > added_lines;

    assert ( line_index < lines.size() );

    std::vector< edge > edge_index_list = get_edges_of_line ( line_index );

    auto it = edge_index_list.begin();
    while ( it != edge_index_list.end() && ( it + 1 ) != edge_index_list.end() )
    {
        edge edge_index = *it;
        edge final_edge_index = * ( it + 1 );
        vertex vertex_index = edges.at ( edge_index ).head_vertex;

        while ( edge_index != final_edge_index )
        {
            line current_edge_line = edges.at ( edge_index ).line_index;
            if ( current_edge_line != line_index )
            {
                added_lines.insert ( current_edge_line );
                result.push_back ( current_edge_line );
            }
            edge_index = get_next_edge ( edge_index, vertex_index );
        }
        ++it;
    }

    if ( result.size() != lines.size() - 1 )
    {
        std::ostringstream out;
        out << "Retrieving a sorted list of lines resulted in a different number of lines than expected. ";
        out << "Expecting " << ( lines.size() - 1 ) << " lines and got " << result.size() << " (";
        std::for_each ( result.begin(), result.end(), [&out] ( line l ) { out << l << ", "; } );
        out << ")";
        throw ues::exc::exception ( out.str(), UES_CONTEXT );
    }
    if ( added_lines.size() != lines.size() - 1 )
    {
        std::ostringstream out;
        out << "Retrieving a sorted list of lines resulted in a list with repeated items (";
        std::for_each ( result.begin(), result.end(), [&out] ( line l ) { out << l << ", "; } );
        out << ")";
        throw ues::exc::exception ( out.str(), UES_CONTEXT );
    }

    return result;
}


void planar_graph::update_edge_references ( planar_graph::edge edge_index, planar_graph::edge old_edge_index )
{
    // Update the edge that shares the head vertex of the edge and should link to this
    // edge (but currently links to the old edge instead).
    const edge_info & ei = edges[edge_index];

    edge current_edge_index = edge_index;
    edge next_edge_index;

    do
    {
        next_edge_index = get_next_edge ( current_edge_index, ei.head_vertex );

        if ( next_edge_index == old_edge_index && current_edge_index != edge_index )
        {
            edge_info & current_edge = edges[current_edge_index];

            assert ( current_edge.head_vertex == ei.head_vertex || current_edge.tail_vertex == ei.head_vertex );

            if ( current_edge.head_vertex == ei.head_vertex )
            {
                current_edge.next_edge = edge_index;
            }
            else
            {
                current_edge.prev_edge = edge_index;
            }

            return;
        }

        current_edge_index = next_edge_index;
    }
    while ( current_edge_index != edge_index );

}


void planar_graph::describe ( std::ostream & out ) const noexcept
{
    if ( edges.size() == 0 )
    {
        out << "Empty graph (no edges)";
    }
    else
    {
        out << "Vertices: ";
        for ( vertex vertex_index = 0; vertex_index < vertices.size(); vertex_index++ )
        {
            out << vertex_index << ' ';
            if ( vertex_index == 0 )
            {
                out << "(infinity)";
            }
            else
            {
                out << vertices[vertex_index].geometric_info;
            }
            out << ", ";
        }

        out << '\n' << "Edges: ";
        for ( edge edge_index = 0; edge_index < edges.size(); edge_index++ )
        {
            out << edge_index << " ( " << edges[edge_index].tail_vertex << " - " << edges[edge_index].head_vertex << " ), ";
        }

        out << '\n' << "Lines:" << '\n';

        for ( line line_index = 0; line_index < lines.size(); line_index++ )
        {
            out << "(" << line_index << ") " << lines[line_index].geometric_info <<  " crosses edges ";
            std::vector< edge > line_edges = get_edges_of_line ( line_index );
            for ( auto it = line_edges.begin(); it != line_edges.end(); ++it )
            {
                out << *it << " ";
            }
            out << '\n';
        }
    }

}


std::ostream & ues::pf::vg2d::operator<< ( std::ostream & out, const planar_graph & g )
{
    g.describe ( out );
    return out;
}
