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

#ifndef UES_PF_VG2D_POINT_SORTER_H
#define UES_PF_VG2D_POINT_SORTER_H

#include <pf/visibility_graph_2d/util/definitions.h>
#include <pf/visibility_graph_2d/point_sort/planar_graph.h>

namespace ues
{
namespace pf
{
namespace vg2d
{

class point_sorter
{
public:
    /** Constructor method. */
    point_sorter ( shared_point_vector points );

    /** Returns a vector of point indices, in the order they are found scanning from the
     * origin point in counter-clockwise order starting at the positive y-axis position. */
    point_index_vector get_sorted_list_of_points ( point_index origin_point ) const;

private:
    shared_point_vector points;
    planar_graph graph;

};

}
}
}

#endif // UES_PF_VG2D_POINT_SORTER_H
