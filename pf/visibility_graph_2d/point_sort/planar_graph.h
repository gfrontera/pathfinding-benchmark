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

#ifndef UES_PF_VG2D_PLANAR_GRAPH_H
#define UES_PF_VG2D_PLANAR_GRAPH_H

#include <vector>

#include <exc/exception.h>
#include <geom/point.h>

#include "line_2d.h"

namespace ues
{
namespace pf
{
namespace vg2d
{

class planar_graph
{
public:
    typedef unsigned int edge;
    typedef unsigned int vertex;
    typedef unsigned int line;

    /** Default constructor */
    planar_graph( ues::math::numeric_type epsilon = ues::math::epsilon ) noexcept;

    /** Adds a line to the graph, inserting all the vertices
     * and edges necessary to keep the graph planar. */
    void add_line ( line_2d );

    /** \name Getter methods */
    /** \{ */

    inline vertex get_edge_tail ( edge ) const;
    inline vertex get_edge_head ( edge ) const;
    
    inline const line_2d& get_line( line line_index ) const;

    /** \} */

    /** Returns the sorted list of edges in the line. It contains all the
     * edges that, together, have the same length as the line,
     * sorted by increasing x coordinate of their vertices. */
    std::vector< edge > get_edges_of_line ( line line_index ) const;
    
    /** Returns a list of lines crossed by given line. All other lines are
     * included in the list, sorted by the x coordinate of the intersection point. */
    std::vector< line > get_sorted_lines( line line_index ) const;

    /** Returns a textual description of the planar graph through the \a out parameter. */
    void describe(std::ostream& out) const noexcept;

private:

    /** \name Edge Information */
    /** \{ */

    struct edge_info
    {
        /** Origin vertex of the edge. */
        vertex tail_vertex;

        /** Terminus vertex of the edge. */
        vertex head_vertex;

        /** The edge adjacent to this edge proceeding
         * counterclockwise around tail vertex. */
        edge prev_edge;

        /** The edge adjacent to this edge proceeding
         * counterclockwise around head vertex. */
        edge next_edge;

        /** The line this edge is part of */
        line line_index;
    };

    std::vector< edge_info > edges;

    /** \} */

    /** \name Vertex Information */
    /** \{ */

    struct vertex_info
    {
        /** Geometrical description of the line */
        geom::point<2> geometric_info;

        /** One of the edges incident to this edge */
        edge header_index;
    };

    /** Vector containing information of vertices. It does not contain
     * information about the vertex 0, so the first element in the vector
     * (vertices[0]) contains information about the second vertex. */
    std::vector< vertex_info > vertices;

    /** \} */

    /** \name Line Information */
    /** \{ */

    struct line_info
    {
        /** Geometrical description of the line */
        line_2d geometric_info;

    };

    std::vector< line_info > lines;

    /** \} */

    /** The precision of the generated graph. */
    ues::math::numeric_type epsilon;

    /** Computes the intersection of an edge and a line. It returns true if
     * and only if they both elements intersect. The intersection point is
     * returned as the result parameter. */
    bool edge_line_intersection ( edge edge_index, line line_index, geom::point<2> & result ) const;

    /** Splits an edge of the graph by a vertex. If the vertex matches one of the limits of the
     * edge, then no splitting is done. Otherwise, the new vertex is added to the graph. The index
     * of the vertex matching the coordinates given by the new vertex is returned. */
    vertex split_edge ( edge edge_index, geom::point<2> new_vertex );

    /** Follows an edge and adds all the new edges of a recently added line.  */
    void follow_line ( edge edge_index, vertex vertex_index, line line_index );

    /** Adds a new edge to the graph between two existing vertices. */
    void add_edge ( vertex vertex1_index, vertex vertex2_index, line line_index );

    /** Updates the edges around a vertex where a new edge has been inserted. */
    void update_edges ( edge new_edge_index, vertex vertex_index );

    /** Returns the next edge found in counterclockwise direction from the provided
     * edge around the provided vertex. */
    edge get_next_edge ( edge edge_index, vertex vertex_index ) const;

    /** Computes the angle of an edge from one of its vertices. Returns a value in the range of
     * [-pi, pi] (in radians). */
    ues::math::numeric_type compute_alpha ( edge edge_index, vertex vertex_index ) const;

    /** Updates the references to a edge that has been replaced by a new one. */
    void update_edge_references ( edge edge_index, edge old_edge_index );

    /** Returns true if the vertex is on the upper side of the line. To determine whether the vertex at
     * infinity is above the line or not, an edge containing the vertex is required. */
    bool upper_side ( edge edge_index, vertex vertex_index, line line_index ) const;
};

/** Output stream operator. */
std::ostream & operator<< ( std::ostream &, const ues::pf::vg2d::planar_graph & );

// Inlined methods

planar_graph::vertex planar_graph::get_edge_head ( planar_graph::edge edge_index ) const
{
    return edges.at ( edge_index ).head_vertex;
}


planar_graph::vertex planar_graph::get_edge_tail ( planar_graph::edge edge_index ) const
{
    return edges.at ( edge_index ).tail_vertex;
}

const line_2d & planar_graph::get_line ( planar_graph::line line_index ) const
{
    return lines.at(line_index).geometric_info;
}


}
}
}

#endif // UES_PF_VG2D_PLANAR_GRAPH_H
