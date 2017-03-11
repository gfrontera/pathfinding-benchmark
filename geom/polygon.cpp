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

#include "polygon.h"

#include <exc/exception.h>

#include "algorithms_2d.h"

using namespace ues::geom;


polygon::polygon ( point_vector points )
    : points ( std::move ( points ) )
{
    if ( this->points.size() < 2 )
    {
        throw ues::exc::exception ( "Polygons must consist of at least 2 points.", UES_CONTEXT );
    }

    update_bounding_box();
}


polygon::polygon ( std::initializer_list< point<2> > il )
    : polygon ( point_vector ( il.begin(), il.end() ) )
{
}


bool polygon::is_inside ( const point<2> & input_point ) const noexcept
{
    // Bounding box check to cheaply discard points.
    if ( bounding_box.contains ( input_point ) )
    {
        segment<2> seg ( input_point, { bounding_box.get_min_x() - 1, input_point.get_y() } );
        point<2> dummy_point;
        return ( get_number_of_intersections ( seg, dummy_point ) % 2 ) == 1;
    }
    return false;
}


bool polygon::check_intersection ( const segment<2> & input_segment, point<2> & intersection_point ) const noexcept
{
    return get_number_of_intersections ( input_segment, intersection_point ) > 0;
}


bool polygon::check_polygon_side_intersection ( const segment<2> & input_segment, const point<2> & first_side_point, const point<2> & second_side_point, point<2> & intersection_point ) const noexcept
{
    if ( check_segment_intersection ( input_segment.get_point_first(), input_segment.get_point_second(), first_side_point, second_side_point, intersection_point ) )
    {
        if ( !input_segment.get_point_first().equal ( intersection_point ) && !input_segment.get_point_second().equal ( intersection_point ) )
        {
            return true;
        }
        else
        {
            const ues::geom::point<2> & other_point = input_segment.get_point_first().equal ( intersection_point ) ?
                                                      input_segment.get_point_second() : input_segment.get_point_first();

            if ( ( second_side_point.get_x() - first_side_point.get_x() ) * ( other_point.get_y() - first_side_point.get_y() )
                    < ( second_side_point.get_y() - first_side_point.get_y() ) * ( other_point.get_x() - first_side_point.get_x() ) )
            {
                return true;
            }

        }
    }

    return false;
}


polygon::point_vector::size_type polygon::get_number_of_intersections ( const segment<2> & seg, point<2> & intersection_point ) const noexcept
{
    point<2> temp_intersection_point;
    point<2> prev_temp_intersection_point;
    bool prev_temp_intersection_valid = check_polygon_side_intersection ( seg, points[points.size() - 1], points[0], prev_temp_intersection_point );

    point_vector::size_type intersection_count = 0;
    for ( point_vector::size_type i = 0; i < points.size(); ++i )
    {
        point_vector::size_type j = ( i + 1 ) % points.size();
        if ( check_polygon_side_intersection ( seg, points[i], points[j], temp_intersection_point ) )
        {
            bool intersects = false;
            if ( !temp_intersection_point.equal ( points[i] ) && !temp_intersection_point.equal ( points[j] ) )
            {
                intersects = true;
            }
            else
            {
                if ( prev_temp_intersection_valid && temp_intersection_point.equal ( prev_temp_intersection_point ) )
                {
                    intersects = true;
                }
            }

            if ( intersects )
            {
                if ( intersection_count > 0 )
                {
                    // If this is not the first intersection found, then use
                    // the point that is nearest to the first point in the segment.
                    if ( seg.get_point_first().distance_to ( temp_intersection_point ) < seg.get_point_first().distance_to ( intersection_point ) )
                    {
                        intersection_point = temp_intersection_point;
                    }
                }
                else
                {
                    // If this is the first intersection found, then use this point.
                    intersection_point = temp_intersection_point;
                }
                ++intersection_count;
            }
            prev_temp_intersection_valid = true;
            prev_temp_intersection_point = temp_intersection_point;
        }
        else
        {
            prev_temp_intersection_valid = false;
        }
    }
    return intersection_count;
}


bool polygon::check_intersection ( const polygon & input_polygon ) const noexcept
{
    if ( bounding_box.intersects ( input_polygon.bounding_box ) )
    {
        return check_polygon_intersection ( *this, input_polygon );
    }
    return false;
}


void polygon::update_bounding_box() noexcept
{
    bounding_box = box_2d ( points[0] );
    for ( point_vector::size_type i = 1; i < points.size(); ++i )
    {
        bounding_box.include_point ( points[i] );
    }
}


polygon & polygon::transform ( const ues::math::matrix & transformation_matrix )
{
    for ( point<2> & p : points )
    {
        p.transform ( transformation_matrix );
    }
    update_bounding_box();
    return *this;
}


bool polygon::operator== ( const polygon & other ) const noexcept
{
    return other.points == points;
}


bool polygon::operator!= ( const polygon & other ) const noexcept
{
    return ! ( *this == other );
}


void polygon::describe ( std::ostream & out ) const noexcept
{
    out << "polygon with vertices ";
    std::for_each ( points.begin(), points.end(), [&out] ( const ues::geom::point<2> & p ) { out << p << " "; } );
}


std::ostream & ues::geom::operator<< ( std::ostream & out, const polygon & poly )
{
    poly.describe ( out );
    return out;
}
