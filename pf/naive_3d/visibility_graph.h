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

#ifndef UES_PF_NAIVE_3D_VISIBILITY_GRAPH_H
#define UES_PF_NAIVE_3D_VISIBILITY_GRAPH_H

#include <unordered_map>

#include <env/obstacle_vector.h>

#include <pf/visibility_graph/visibility_graph.h>
#include <misc/cache.h>

namespace ues
{
namespace pf
{
namespace naive_3d
{

class visibility_graph : public ues::pf::visibility_graph<3>
{
public:
    /** Type used for point indices. */
    typedef typename ues::pf::visibility_graph<3>::size_type size_type;

    static const ues::math::numeric_type MAXIMUM_POINT_SEPARATION;

    /** \name Constructor methods */
    /** \{ */

    visibility_graph ( ues::env::obstacle_vector obstacles,
                       ues::geom::point<3> origin,
                       ues::geom::point<3> target );

    visibility_graph ( ues::env::obstacle_vector obstacles,
                       ues::geom::point<3> origin,
                       ues::geom::point<3> target,
                       unsigned int cache_size );

    /** \} */

    /** Destructor method. */
    virtual ~visibility_graph() = default;

    /** Returns the number of points in the graph. */
    size_type size() const noexcept override;

    /** Prints the visibility matrix to the \a out parameter. */
    void describe ( std::ostream & out ) const noexcept override;

private:
    typedef std::vector< ues::geom::point<3> > point_vector;
    typedef std::unordered_map<ues::geom::point<3>, size_type> point_map;
    typedef typename ues::pf::visibility_graph<3>::point_index_vector point_index_vector;
    typedef std::pair<size_type, size_type> point_index_pair;

    struct index_pair_hash
    {
        std::size_t operator() ( const point_index_pair & ) const noexcept;
    };

    typedef ues::misc::cache< point_index_pair, bool, index_pair_hash > index_cache;

    ues::env::obstacle_vector obstacles;
    point_vector points;
    point_map point_indices;
    mutable index_cache cache;

    /** Returns the index assigned to a point. */
    const geom::point<3> & index_to_point ( size_type point_index ) const override;
    /** Returns the point assigned to an index. */
    size_type point_to_index ( const geom::point<3> & point ) const override;

    /** Returns true if the points of indices \a point1_index and \a point2_index are visible
     * from one another, and false otherwise. If points are visible, the distance between the
     * points is returned in the variable \a distance. */
    bool check_visibility ( size_type point1_index, size_type point2_index, ues::math::numeric_type & distance ) const override;

    /** Returns a vector containing all the point indices corresponding to points that are
     * visible from the point of index \a point_index. */
    point_index_vector adjacents ( size_type point_index ) const override;

    /** Inserts a point as a vertex of the graph. */
    bool insert_point ( ues::geom::point<3> point );

    /** Initialization of the point vector. */
    void init();
};

}
}
}

#endif // UES_PF_NAIVE_3D_VISIBILITY_GRAPH_H
