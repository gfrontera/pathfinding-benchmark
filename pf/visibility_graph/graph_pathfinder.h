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

#ifndef UES_PF_GRAPH_PATHFINDER_H
#define UES_PF_GRAPH_PATHFINDER_H

#include <pf/path.h>
#include <pf/visibility_graph/visibility_graph.h>

namespace ues
{
namespace pf
{

template<unsigned short N>
class graph_pathfinder
{
public:
    /** Constructor method receiving a visibility graph. */
    graph_pathfinder ( std::shared_ptr< visibility_graph<N> > graph );

    /** Finds a path between two points using the provided visibility graph. */
    path<N> find_path ( const ues::geom::point<N> & origin,
                        const ues::geom::point<N> & target );

private:
    std::shared_ptr< visibility_graph<N> > graph;
};

}
}

#endif // UES_PF_GRAPH_PATHFINDER_H
