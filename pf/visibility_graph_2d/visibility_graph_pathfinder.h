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

#ifndef UES_PF_VG2D_VISIBILITY_GRAPH_PATHFINDER_H
#define UES_PF_VG2D_VISIBILITY_GRAPH_PATHFINDER_H

#include <geom/polygon.h>
#include <pf/path.h>

namespace ues
{
namespace pf
{
namespace vg2d
{

class visibility_graph_pathfinder
{
public:
    static ues::pf::path<2> find_path ( const std::vector< ues::geom::polygon > & obstacles,
                                        const ues::geom::point<2> & origin,
                                        const ues::geom::point<2> & target );
};

}
}
}

#endif // UES_PF_VG2D_VISIBILITY_GRAPH_PATHFINDER_H
