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

#ifndef UES_PF_VG2D_VISIBILITY_GRAPH_2D_H
#define UES_PF_VG2D_VISIBILITY_GRAPH_2D_H

#include <pf/visibility_graph/basic_visibility_graph.h>
#include <pf/visibility_graph_2d/util/definitions.h>


namespace ues
{
namespace pf
{

namespace vg2d
{

class visibility_graph : public ues::pf::basic_visibility_graph<2>
{
public:
    /** Constructor method. */
    visibility_graph ( shared_point_vector pv, shared_segment_vector sv = std::make_shared< segment_vector >() ) noexcept;

    /** Returns true if the point is already in the graph, false otherwise. */
    bool has_point ( const geom::point<2> & point ) const override;

    /** Adds information on what is the segment occluding the vision between points origin and target. */
    void add_occlusion_segment ( const ues::geom::point<2> & origin, const ues::geom::point<2> & target, segment_index segment );

    /** Returns the segment occluding the vision between points origin and target. */
    bool check_occlusion_segment ( const ues::geom::point<2> & origin, const ues::geom::point<2> & target, segment_index & occluding_segment ) const;

    /** Prints the visibility matrix to the \a out parameter. */
    void describe ( std::ostream & out ) const noexcept override;
private:
    typedef std::unordered_map< ues::geom::point<2>, point_index > point_indices;
    typedef std::unordered_map< point_index, segment_index > occluding_segments;
    typedef std::vector< occluding_segments > occluding_segment_vector;

    point_indices pti;
    occluding_segment_vector osv;
    shared_point_vector pv;
    shared_segment_vector sv;

    size_type point_to_index ( const ues::geom::point<2> & point ) const override;
    const geom::point<2> & index_to_point ( size_type point_index ) const override;

    void add_occlusion_segment ( point_index origin, point_index target, segment_index segment );

    bool check_occlusion_segment ( point_index origin, point_index target, segment_index & occluding_segment ) const;
};

}
}
}


#endif // UES_PF_VG2D_VISIBILITY_GRAPH_2D_H



