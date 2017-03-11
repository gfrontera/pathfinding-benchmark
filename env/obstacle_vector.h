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

#ifndef UES_ENV_OBSTACLE_VECTOR_H
#define UES_ENV_OBSTACLE_VECTOR_H

#include <vector>

#include "obstacle.h"

namespace ues
{
namespace env
{

class obstacle_vector : public std::vector< obstacle >
{
public:
    using std::vector< obstacle >::vector;

    /** Returns \c true if the \a point is inside any of the obstacles, \c false otherwise. */
    bool contains_point ( const ues::geom::point<3> & ) const noexcept;

    /** Checks whether the \c input_segment intersects any obstacle. If there is an intersection,
     * this method returns \c true and the \c intersection_point nearest to the first point of the
     * segment. */
    bool check_intersection ( const ues::geom::segment<3> & input_segment, ues::geom::point<3> & intersection_point ) const noexcept;

    /** Checks whether the \c input_segment intersects any obstacle. If there is an intersection,
     * this method returns \c true. */
    bool check_intersection ( const ues::geom::segment<3> & input_segment ) const noexcept;
};

/** Output stream operator. */
std::ostream & operator<< ( std::ostream &, const ues::env::obstacle_vector & );

}
}

#endif // UES_ENV_OBSTACLE_VECTOR_H
