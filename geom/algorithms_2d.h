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

#ifndef UES_GEOM_ALGORITHMS_2D_H
#define UES_GEOM_ALGORITHMS_2D_H

#include "point.h"
#include "polygon.h"

namespace ues
{
namespace geom
{

point<2> segment_intersection ( const point<2> & segment1_point1,
                                const point<2> & segment1_point2,
                                const point<2> & segment2_point1,
                                const point<2> & segment2_point2,
                                const ues::math::numeric_type & epsilon = ues::math::epsilon );

bool check_segment_intersection ( const point<2> & segment1_point1,
                                  const point<2> & segment1_point2,
                                  const point<2> & segment2_point1,
                                  const point<2> & segment2_point2,
                                  point<2> & result,
                                  const ues::math::numeric_type & epsilon = ues::math::epsilon ) noexcept;

ues::math::numeric_type point_to_segment_distance ( const point<2> & origin_point,
                                         const point<2> & segment_point1,
                                         const point<2> & segment_point2,
                                         const ues::math::numeric_type & angle,
                                         const ues::math::numeric_type & epsilon = ues::math::epsilon );

bool check_polygon_intersection ( const polygon & poly1, const polygon & poly2 ) noexcept;

/** Returns a homogeneous translation matrix corresponding to the vector with coordinates \a x and \a y. */
ues::math::matrix translation_matrix_2d ( const ues::math::numeric_type & x, const ues::math::numeric_type & y ) noexcept;

/** Returns a homogeneous rotation matrix of a given \a angle. */
ues::math::matrix rotation_matrix_2d ( const ues::math::numeric_type & angle ) noexcept;

}
}

#endif // UES_GEOM_ALGORITHMS_2D_H
