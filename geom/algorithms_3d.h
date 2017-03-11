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

#ifndef UES_GEOM_ALGORITHMS_3D_H
#define UES_GEOM_ALGORITHMS_3D_H

#include <math/definitions.h>

#include "segment.h"
#include "point.h"

namespace ues
{
namespace geom
{

/** Checks whether the segment \a input_segment and the horizontal plane defined
 * by the equation z = \a plane_height intersect. If an intersection point exists,
 * this function returns \c true and will fill the value of \a intersection_point. */
bool check_segment_horizontal_plane_intersection ( const segment<3> & input_segment, const ues::math::numeric_type & plane_height, point<3> & intersection_point ) noexcept;

/** Returns the intersection between the \a input_segment and the horizontal plane defined
 * by the equation z = \a plane_height. If an intersection point doesn't exist,
 * this throws an exception. */
point<3> segment_horizontal_plane_intersection ( const segment<3> & input_segment, const ues::math::numeric_type & plane_height );

/** Returns a 2D point discarding the z coordinate. */
point<2> point_3d_to_2d( const point<3>& input_point ) noexcept;

/** Returns a 3D point adding a z coordinate with the given \a value. */
point<3> point_2d_to_3d( const point<2>& input_point, const ues::math::numeric_type& value = 0 ) noexcept;

/** Returns a homogeneous translation matrix corresponding to the vector with coordinates \a x, \a y and \a z. */
ues::math::matrix translation_matrix_3d ( const ues::math::numeric_type & x, const ues::math::numeric_type & y, const ues::math::numeric_type & z ) noexcept;

/** Returns a homogeneous rotation matrix of \a value radians around the \a axis. */
ues::math::matrix rotation_matrix_3d ( const ues::math::numeric_type & value, ues::math::vector axis );

}
}

#endif // UES_GEOM_ALGORITHMS_3D_H
