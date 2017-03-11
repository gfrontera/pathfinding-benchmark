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

#ifndef UES_PF_PATHFINDER_H
#define UES_PF_PATHFINDER_H

#include <env/obstacle_vector.h>

#include <pf/path.h>

namespace ues
{
namespace pf
{

class pathfinder
{
public:
    /** Destructor method. */
    virtual ~pathfinder() noexcept = default;

    /** Generates an approximately-shortest path from \a origin to \a target, given some \a obstacles. */
    virtual path<3> find_path ( const ues::env::obstacle_vector & obstacles,
                                const ues::geom::point<3> & origin,
                                const ues::geom::point<3> & target ) const = 0;

    /** \name Clone methods */
    /** \{ */
    /** Returns a pointer to a copy of this \c pathfinder object. */
    virtual pathfinder * clone() const & = 0;
    /** Returns a pointer to a copy of this \c pathfinder object, destroying the original. */
    virtual pathfinder * clone() && = 0;
    /** \} */
};

pathfinder * new_clone ( const pathfinder & );
pathfinder * new_clone ( pathfinder && );

}
}

#endif // UES_PF_PATHFINDER_H
