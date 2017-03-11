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

#ifndef UES_PF_VISIBILITY_GRAPH_H
#define UES_PF_VISIBILITY_GRAPH_H

#include <geom/point.h>

namespace ues
{
namespace pf
{

template<unsigned short N>
class graph_pathfinder; /* Forward declaration. */

template<unsigned short N>
class visibility_graph
{
public:
    typedef std::size_t size_type; /**< Type used for point indices. */

    /** Destructor method. */
    virtual ~visibility_graph() = default;

    /** Returns true if the points \a point1 and \a point2 are visible from one another, and
     * false otherwise. If points are visible, the distance between the points is returned in
     * the variable \a distance. */
    inline bool check_visibility ( const ues::geom::point<N> & point1,
                            const ues::geom::point<N> & point2,
                            ues::math::numeric_type & distance ) const;

    /** Returns the number of points in the graph. */
    virtual size_type size() const noexcept = 0;

    /** Prints the visibility matrix to the \a out parameter. */
    virtual void describe ( std::ostream & out ) const noexcept = 0;
protected:
    friend class graph_pathfinder<N>;

    typedef std::vector< size_type > point_index_vector;

    /** Returns the index assigned to a point. */
    virtual size_type point_to_index ( const ues::geom::point<N> & point ) const = 0;
    /** Returns the point assigned to an index. */
    virtual const ues::geom::point<N> & index_to_point ( size_type point_index ) const = 0;

    /** Returns true if the points of indices \a point1_index and \a point2_index are visible
     * from one another, and false otherwise. If points are visible, the distance between the
     * points is returned in the variable \a distance. */
    virtual bool check_visibility ( size_type point1_index,
                                    size_type point2_index,
                                    ues::math::numeric_type & distance ) const = 0;

    /** Returns a vector containing all the point indices corresponding to points that are
     * visible from the point of index \a point_index. */
    virtual point_index_vector adjacents ( size_type point_index ) const = 0;
};


/** Output stream operator. */
template<unsigned short N>
inline std::ostream & operator<< ( std::ostream & out, const ues::pf::visibility_graph<N> & graph ) noexcept;


// Template implementation.


template<unsigned short N>
bool visibility_graph<N>::check_visibility ( const ues::geom::point<N> & point1,
                                             const ues::geom::point<N> & point2,
                                             ues::math::numeric_type & distance ) const
{
    size_type point1_index = point_to_index ( point1 );
    size_type point2_index = point_to_index ( point2 );

    return check_visibility ( point1_index, point2_index, distance );
}


/** Output stream operator. */
template<unsigned short N>
std::ostream & operator<< ( std::ostream & out, const ues::pf::visibility_graph<N> & graph ) noexcept
{
    graph.describe ( out );
    return out;
}


}
}

#endif // UES_PF_VISIBILITY_GRAPH_H
