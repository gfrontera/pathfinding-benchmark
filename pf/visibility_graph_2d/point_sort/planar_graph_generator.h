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

#ifndef UES_PF_VG2D_PLANAR_GRAPH_GENERATOR_H
#define UES_PF_VG2D_PLANAR_GRAPH_GENERATOR_H

#include <geom/point.h>
#include <exc/exception.h>

#include <pf/visibility_graph_2d/util/definitions.h>
#include <pf/visibility_graph_2d/point_sort/planar_graph.h>
#include <pf/visibility_graph_2d/point_sort/line_2d.h>

namespace ues
{
namespace pf
{
namespace vg2d
{

class planar_graph_generator
{
public:
    /** Generates a planar graph from a vector of points. */
    static planar_graph generate_planar_graph ( const point_vector & points, const ues::math::numeric_type & epsilon = ues::math::epsilon );

    /** Computes the transform of the given point to its corresponding line using the principles of
     * geometric duality */
    static line_2d transform ( const ues::geom::point<2> & pt ) noexcept;

    /** Computes the transform of the given line to its corresponding point using the principles of
      * geometric duality */
    static ues::geom::point<2> transform ( const line_2d & pt ) noexcept;
};

}
}
}

#endif // UES_PF_VG2D_PLANAR_GRAPH_GENERATOR_H
