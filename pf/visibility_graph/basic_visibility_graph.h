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

#ifndef UES_PF_BASIC_VISIBILITY_GRAPH_H
#define UES_PF_BASIC_VISIBILITY_GRAPH_H

#include <memory>
#include <unordered_map>

#include <pf/visibility_graph/visibility_graph.h>

namespace ues
{
namespace pf
{

template<unsigned short N>
class basic_visibility_graph : public visibility_graph<N>
{
public:
    typedef typename visibility_graph<N>::size_type size_type; /**< Type used for point indices. */

    /** Default constructor method. */
    basic_visibility_graph() noexcept;

    /** Constructor method. */
    basic_visibility_graph ( size_type number_of_points ) noexcept;

    /** Destructor method. */
    virtual ~basic_visibility_graph() noexcept;

    /** Adds visibility information between points \a point1 and \a point2. The distance between
     * both points is computed internally. */
    void add_visibility ( const ues::geom::point<N> & point1,
                          const ues::geom::point<N> & point2 );

    /** Adds visibility information between points \a point1 and \a point2. The distance between
     * both points is provided in \a distance. */
    void add_visibility ( const ues::geom::point<N> & point1,
                          const ues::geom::point<N> & point2,
                          ues::math::numeric_type distance );

    /** Returns the number of points in the graph. */
    size_type size() const noexcept override;

    /** Returns true if the point is already in the graph, false otherwise. */
    virtual bool has_point ( const ues::geom::point<N> & point ) const = 0;

    /** Prints the visibility matrix to the \a out parameter. */
    virtual void describe ( std::ostream & out ) const noexcept override;

    /** Reintroduce hidden overloads. */
    using visibility_graph<N>::check_visibility;

protected:
    /** Makes room for another point in the graph. */
    void add_point();

private:
    typedef typename visibility_graph<N>::point_index_vector point_index_vector;
    typedef std::unordered_map< size_type, ues::math::numeric_type > point_visibility;
    typedef std::vector< point_visibility > visibility_vector;

    size_type number_of_points;
    visibility_vector vv;

    /** Returns the index assigned to a point. */
    virtual size_type point_to_index ( const ues::geom::point<N> & point ) const override = 0;
    /** Returns the point assigned to an index. */
    virtual const ues::geom::point<N> & index_to_point ( size_type point_index ) const override = 0;

    /** Adds visibility information between points of indices \a point1_index and \a point2_index.
     * The distance between both points is provided in \a distance. */
    void add_visibility ( size_type point1_index,
                          size_type point2_index,
                          ues::math::numeric_type distance );

    /** Returns true if the points of indices \a point1_index and \a point2_index are visible
     * from one another, and false otherwise. If points are visible, the distance between the
     * points is returned in the variable \a distance. */
    bool check_visibility ( size_type point1_index,
                            size_type point2_index,
                            ues::math::numeric_type & distance ) const override;

    /** Returns a vector containing all the point indices corresponding to points that are
     * visible from the point of index \a point_index. */
    point_index_vector adjacents ( size_type point_index ) const override;
};

}
}

#endif // UES_PF_BASIC_VISIBILITY_GRAPH_H
