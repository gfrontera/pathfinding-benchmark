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

#include "algorithms_3d.h"

#include <cassert>

#include <exc/exception.h>

const std::string component_name = "3D Geometric Algorithms";

using namespace ues::geom;


bool ues::geom::check_segment_horizontal_plane_intersection ( const segment<3> & input_segment,
                                                              const ues::math::numeric_type & plane_height,
                                                              point<3> & intersection_point ) noexcept
{
    // The intersection only exists if the segment crosses the plane's height.
    if ( ( ( input_segment.get_point_first().get_z() > plane_height ) ^ ( input_segment.get_point_second().get_z() > plane_height ) ) ||
    ( ( input_segment.get_point_first().get_z() < plane_height ) ^ ( input_segment.get_point_second().get_z() < plane_height ) ) )
    {
        ues::math::numeric_type zdiff_inv = 1 / std::abs ( input_segment.get_point_first().get_z() - input_segment.get_point_second().get_z() );
        ues::math::numeric_type first_proximity = std::abs ( input_segment.get_point_first().get_z() - plane_height ) * zdiff_inv;
        ues::math::numeric_type second_proximity = std::abs ( input_segment.get_point_second().get_z() - plane_height ) * zdiff_inv;

        assert ( std::abs ( first_proximity + second_proximity - 1 ) < ues::math::epsilon );

        intersection_point = point<3> ( input_segment.get_point_first().get_x() * second_proximity + input_segment.get_point_second().get_x() * first_proximity,
        input_segment.get_point_first().get_y() * second_proximity + input_segment.get_point_second().get_y() * first_proximity,
        plane_height );

        return true;
    }
    else
    {
        return false;
    }
}


point<3> ues::geom::segment_horizontal_plane_intersection ( const segment<3> & input_segment, const ues::math::numeric_type & plane_height )
{
    ues::geom::point<3> result;

    if ( ues::geom::check_segment_horizontal_plane_intersection ( input_segment, plane_height, result ) )
    {
        return result;
    }
    else
    {
        throw ues::exc::exception ( "Segment intersection does not exist", UES_CONTEXT );
    }
}


ues::math::matrix ues::geom::translation_matrix_3d ( const ues::math::numeric_type & x, const ues::math::numeric_type & y, const ues::math::numeric_type & z ) noexcept
{
    // A 4x4 matrix for homogeneous 3D transformation
    return ues::math::matrix::fixed<4, 4> { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1 };
}

ues::math::matrix ues::geom::rotation_matrix_3d ( const ues::math::numeric_type & val, ues::math::vector axis )
{
    if ( axis.n_rows != 3 )
        throw ues::exc::exception ( "The axis must be a 3D unitary column vector.", UES_CONTEXT );

    const ues::math::numeric_type sinv = std::sin ( val );
    const ues::math::numeric_type cosv = std::cos ( val );

    const ues::math::numeric_type & ux = axis[0];
    const ues::math::numeric_type & uy = axis[1];
    const ues::math::numeric_type & uz = axis[2];

    // Check the axis is unitary
    assert ( std::abs ( ux * ux + uy * uy + uz * uz - 1 ) < ues::math::epsilon );

    return ues::math::matrix::fixed<4, 4> { cosv + ux * ux * ( 1 - cosv ), uy * ux * ( 1 - cosv ) + uz * sinv, uz * ux * ( 1 - cosv ) - uy * sinv, 0,
                                            ux * uy * ( 1 - cosv ) - uz * sinv, cosv + uy * uy * ( 1 - cosv ), uz * uy * ( 1 - cosv ) + ux * sinv, 0,
                                            ux * uz * ( 1 - cosv ) + uy * sinv, uy * uz * ( 1 - cosv ) - ux * sinv, cosv + uz * uz * ( 1 - cosv ), 0,
                                            0, 0, 0, 1
                                          };

}


point<3> ues::geom::point_2d_to_3d ( const point<2> & input_point, const ues::math::numeric_type & value ) noexcept
{
    return point<3> ( input_point.get_x(), input_point.get_y(), value );
}


point<2> ues::geom::point_3d_to_2d ( const point<3> & input_point ) noexcept
{
    return point<2> ( input_point.get_x(), input_point.get_y() );
}
