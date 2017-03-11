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

#include "obstacle.h"

#include <cassert>

#include <exc/exception.h>
#include <geom/algorithms_3d.h>

using namespace ues::env;


obstacle::obstacle ( ues::geom::polygon shape, ues::math::numeric_type height )
    : shape ( std::move ( shape ) ),
      height ( std::move ( height ) )
{
    if ( height <= 0 )
        throw ues::exc::exception ( "Obstacles must have a height greater than zero", UES_CONTEXT );
}


bool obstacle::check_intersection ( const ues::geom::segment<3> & input_segment, ues::geom::point<3> & intersection_point ) const noexcept
{
    // If both points of the segment are above the obstacle, then there is no intersection.
    if ( input_segment.get_point_first().get_z() >= height && input_segment.get_point_second().get_z() >= height )
    {
        return false;
    }
    else
    {
        bool intersects = false;

        // First, check if the segment intersects the obstacle from above.
        {
            ues::geom::point<3> temp_intersection_point;
            if ( ues::geom::check_segment_horizontal_plane_intersection ( input_segment, height, temp_intersection_point ) )
            {
                ues::geom::point<2> temp_2d_point ( temp_intersection_point.get_x(), temp_intersection_point.get_y() );
                if ( shape.is_inside ( temp_2d_point ) )
                {
                    intersection_point = temp_intersection_point;
                    intersects = true;
                }

            }
        }

        // If the segment doesn't intersect the obstacle from above, it still can intersect it from a side.
        {
            ues::geom::point<2> intersection_point_2d;
            ues::geom::segment<2> segment_projection ( ues::geom::point<2> ( input_segment.get_point_first().get_x(), input_segment.get_point_first().get_y() ),
                                                       ues::geom::point<2> ( input_segment.get_point_second().get_x(), input_segment.get_point_second().get_y() ) );

            if ( shape.check_intersection ( segment_projection, intersection_point_2d ) )
            {
                // Check the height of the intersection point to see if it is actually an intersection.
                ues::math::numeric_type seg_length_inv = 1 / segment_projection.get_point_first().distance_to ( segment_projection.get_point_second() );
                ues::math::numeric_type first_half_length = segment_projection.get_point_first().distance_to ( intersection_point_2d ) * seg_length_inv;
                ues::math::numeric_type second_half_length = segment_projection.get_point_second().distance_to ( intersection_point_2d ) * seg_length_inv;

                assert ( std::abs ( first_half_length + second_half_length - 1 ) < ues::math::epsilon );

                ues::math::numeric_type intersection_z = input_segment.get_point_first().get_z() * second_half_length +
                                                         input_segment.get_point_second().get_z() * first_half_length;

                if ( intersection_z < height + ues::math::epsilon )
                {
                    ues::geom::point<3> temp_intersection_point = { intersection_point_2d.get_x(), intersection_point_2d.get_y(), intersection_z };
                    if ( !intersects || input_segment.get_point_first().distance_to ( intersection_point ) > input_segment.get_point_first().distance_to ( temp_intersection_point ) )
                    {
                        intersection_point = temp_intersection_point;
                    }
                    intersects = true;
                }
            }
        }

        // If the segment doesn't intersect the obstacle from above or from a side, return false.
        return intersects;

    }
}


bool obstacle::contains_point ( const ues::geom::point<3> & point ) const noexcept
{
    // If point is above the obstacle, then there is no intersection.
    if ( point.get_z() >= height )
    {
        return false;
    }
    else
    {
        return shape.is_inside ( ues::geom::point_3d_to_2d ( point ) );
    }
}


bool obstacle::operator== ( const obstacle & other ) const noexcept
{
    return other.height == height && other.shape == shape;
}


bool obstacle::operator!= ( const obstacle & other ) const noexcept
{
    return ! ( *this == other );
}


void obstacle::describe ( std::ostream & out ) const noexcept
{
    out << "obstacle of " << shape << "and height of " << height;
}


std::ostream & ues::env::operator<< ( std::ostream & out, const obstacle & o )
{
    o.describe ( out );
    return out;
}
