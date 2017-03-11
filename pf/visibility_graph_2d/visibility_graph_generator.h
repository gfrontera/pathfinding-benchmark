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

#ifndef UES_PF_VG2D_VISIBILITY_GRAPH_GENERATOR_H
#define UES_PF_VG2D_VISIBILITY_GRAPH_GENERATOR_H

#include <pf/visibility_graph_2d/util/definitions.h>
#include <pf/visibility_graph_2d/visibility_graph.h>
#include <pf/visibility_graph_2d/point_sort/point_sorter.h>

namespace ues
{
namespace pf
{
namespace vg2d
{

class visibility_graph_generator
{
public:
    visibility_graph_generator ( shared_point_vector points );

    std::shared_ptr< visibility_graph > generate_visibility_graph ( const shared_segment_vector & segments,
                                                                    const shared_polygon_vector & polygons );

private:
    shared_point_vector points;
    point_sorter sorter;
};

}
}
}

#endif // UES_PF_VG2D_VISIBILITY_GRAPH_GENERATOR_H
