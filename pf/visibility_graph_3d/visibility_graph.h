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

#ifndef UES_PF_VG3D_VISIBILITY_GRAPH_H
#define UES_PF_VG3D_VISIBILITY_GRAPH_H

#include <pf/visibility_graph/basic_visibility_graph.h>

#include "definitions.h"

namespace ues
{
namespace pf
{

namespace vg3d
{

class visibility_graph : public ues::pf::basic_visibility_graph<3>
{
public:
    /** Default constructor method. */
    visibility_graph() noexcept;
    /** Constructor method. */
    visibility_graph ( point_vector pv ) noexcept;

    /** Returns true if the point is already in the graph, false otherwise. */
    bool has_point ( const ues::geom::point<3> & point ) const override;

    /** Adds a new point to the graph. The point provided must not be in the graph. */
    void add_point ( ues::geom::point<3> point );

private:
    typedef std::unordered_map< ues::geom::point<3>, point_index > point_indices;

    point_indices pti;
    point_vector pv;

    /** Returns the index assigned to a point. */
    size_type point_to_index ( const ues::geom::point<3> & new_point ) const override;

    /** Returns the point assigned to an index. */
    const ues::geom::point<3> & index_to_point ( size_type point_index ) const override;
};

}
}
}

#endif // UES_PF_VG3D_VISIBILITY_GRAPH_3D_H

