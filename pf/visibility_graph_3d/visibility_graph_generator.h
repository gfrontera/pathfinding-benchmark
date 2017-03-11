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

#ifndef UES_PF_VG3D_VISIBILITY_GRAPH_GENERATOR_H
#define UES_PF_VG3D_VISIBILITY_GRAPH_GENERATOR_H

#include <env/obstacle_vector.h>
#include <geom/point.h>

#include <pf/visibility_graph_3d/visibility_graph.h>

namespace ues
{
namespace pf
{
namespace vg3d
{

class visibility_graph_generator
{
public:
    static const unsigned int MAXIMUM_HEIGHTS;

    static std::shared_ptr< visibility_graph > generate_visibility_graph ( const ues::env::obstacle_vector & obstacles,
                                                                           const ues::geom::point<3> & origin,
                                                                           const ues::geom::point<3> & target );

};

}
}
}

#endif // UES_PF_VG3D_VISIBILITY_GRAPH_GENERATOR_H
