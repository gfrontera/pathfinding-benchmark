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

#ifndef UES_PF_MP_BITSTAR_PATHFINDER_H
#define UES_PF_MP_BITSTAR_PATHFINDER_H

#include <pf/pathfinder.h>

namespace ues
{
namespace pf
{
namespace mp
{

class bitstar_pathfinder : public ues::pf::pathfinder
{
public:
    ues::pf::path< 3 > find_path ( const ues::env::obstacle_vector & obstacles,
                                   const ues::geom::point<3> & origin,
                                   const ues::geom::point<3> & target ) const override;

    /** \name Clone methods */
    /** \{ */
    bitstar_pathfinder * clone() const & override;
    bitstar_pathfinder * clone() && override;
    /** \} */

};

}
}
}

#endif // UES_PF_MP_BITSTAR_PATHFINDER_H
