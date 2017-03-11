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

#ifndef UES_ENV_OBSTACLE_H
#define UES_ENV_OBSTACLE_H

#include <geom/polygon.h>
#include <geom/segment.h>
#include <geom/point.h>

namespace ues
{
namespace env
{

class obstacle
{
public:
    /** Constructor method. */
    obstacle ( ues::geom::polygon shape, ues::math::numeric_type height );

    /** Outputs a textual description of the obstacle to a ostream object. */
    void describe ( std::ostream & out ) const noexcept;

    /** Checks whether the \c input_segment intersects the obstacle. If there is an intersection,
     * this method returns \c true and the \c intersection_point nearest to the first point of the
     * segment. */
    bool check_intersection ( const ues::geom::segment<3> & input_segment, ues::geom::point<3> & intersection_point ) const noexcept;

    /** Returns \c true if the \a point is inside the obstacle, \c false otherwise. */
    bool contains_point ( const ues::geom::point<3> & point ) const noexcept;

    /** \name Getter and setter methods */
    /** \{ */

    inline const ues::geom::polygon & get_shape() const noexcept;
    inline const ues::math::numeric_type & get_height() const noexcept;

    /** \} */


    /** \name Operators */
    /** \{ */

    bool operator== ( const obstacle & ) const noexcept;
    bool operator!= ( const obstacle & ) const noexcept;

    /** \} */

private:
    ues::geom::polygon shape;
    ues::math::numeric_type height;
};

/** Output stream operator. */
std::ostream & operator<< ( std::ostream &, const ues::env::obstacle & );

// Inlined methods.


const geom::polygon & obstacle::get_shape() const noexcept
{
    return shape;
}


const ues::math::numeric_type & obstacle::get_height() const noexcept
{
    return height;
}


}
}

#endif // UES_ENV_OBSTACLE_H
