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

#ifndef UES_GEOM_BOX_2D_H
#define UES_GEOM_BOX_2D_H

#include "math.h"
#include "point.h"

namespace ues
{
namespace geom
{

class box_2d
{
public:
    box_2d() noexcept;

    box_2d ( ues::math::numeric_type min_x, ues::math::numeric_type min_y,
             ues::math::numeric_type max_x, ues::math::numeric_type max_y );

    box_2d ( const ues::geom::point<2> & ) noexcept;

    /** \name Getter methods */
    /** \{ */
    inline const ues::math::numeric_type & get_min_x() const noexcept;
    inline const ues::math::numeric_type & get_max_x() const noexcept;
    inline const ues::math::numeric_type & get_min_y() const noexcept;
    inline const ues::math::numeric_type & get_max_y() const noexcept;
    /** \} */

    /** Stretches the box to include a point. */
    void include_point ( const ues::geom::point<2> & ) noexcept;

    /** Returns \c true if the bounding box contains a point. */
    bool contains ( const ues::geom::point<2> & ) const noexcept;

    /** Returns \c true if the two bounding boxes intersect. */
    bool intersects ( const ues::geom::box_2d & ) const noexcept;

private:
    ues::math::numeric_type min_x, min_y, max_x, max_y;
};

// Inlined methods.

const ues::math::numeric_type & box_2d::get_min_x() const noexcept
{
    return min_x;
}


const ues::math::numeric_type & box_2d::get_min_y() const noexcept
{
    return min_y;
}


const ues::math::numeric_type & box_2d::get_max_x() const noexcept
{
    return max_x;
}


const ues::math::numeric_type & box_2d::get_max_y() const noexcept
{
    return max_y;
}


}
}

#endif // UES_GEOM_BOX_2D_H
