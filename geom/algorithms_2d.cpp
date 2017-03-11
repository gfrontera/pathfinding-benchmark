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

#include "algorithms_2d.h"

#include <boost/geometry.hpp>

#include <log/logger.h>
#include <exc/exception.h>

#include "polygon_traits.h"

using namespace ues::geom;

const std::string component_name = "2D Geometric Algorithms";


point<2> ues::geom::segment_intersection ( const point<2> & s1p1,
                                           const point<2> & s1p2,
                                           const point<2> & s2p1,
                                           const point<2> & s2p2,
                                           const ues::math::numeric_type & epsilon )
{
    point<2> result;
    if ( check_segment_intersection ( s1p1, s1p2, s2p1, s2p2, result, epsilon ) )
    {
        return result;
    }
    else
    {
        std::ostringstream error;
        error << "No intersection found between segments ( " << s1p1 << ", " << s1p2 << " ) and ( " << s2p1 << ", " << s2p2 << " )";
        throw ues::exc::exception ( error.str(), UES_CONTEXT );
    }
}


bool ues::geom::check_segment_intersection ( const point<2> & s1p1,
                                             const point<2> & s1p2,
                                             const point<2> & s2p1,
                                             const point<2> & s2p2,
                                             point<2> & result,
                                             const ues::math::numeric_type & epsilon ) noexcept
{
    ues::log::logger lg;

    const ues::math::numeric_type & x1 = s1p1.get_x(), & y1 = s1p1.get_y(),
    & x2 = s1p2.get_x(), & y2 = s1p2.get_y(),
    & x3 = s2p1.get_x(), & y3 = s2p1.get_y(),
    & x4 = s2p2.get_x(), & y4 = s2p2.get_y();

    ues::math::numeric_type denominator = ( x1 - x2 ) * ( y3 - y4 ) - ( y1 - y2 ) * ( x3 - x4 );

    if ( denominator != 0 )
    {
        ues::math::numeric_type denominator_inv = 1 / denominator;
        ues::math::numeric_type first_factor = ( x1 * y2 - y1 * x2 );
        ues::math::numeric_type second_factor = ( x3 * y4 - y3 * x4 );
        ues::math::numeric_type x, y;
        x = ( first_factor * ( x3 - x4 ) - ( x1 - x2 ) * second_factor ) * denominator_inv;
        y = ( first_factor * ( y3 - y4 ) - ( y1 - y2 ) * second_factor ) * denominator_inv;

        result = { x, y };

        if ( x + epsilon >= std::max ( std::min ( x1, x2 ), std::min ( x3, x4 ) ) &&
                x - epsilon <= std::min ( std::max ( x1, x2 ), std::max ( x3, x4 ) ) &&
                y + epsilon >= std::max ( std::min ( y1, y2 ), std::min ( y3, y4 ) ) &&
                y - epsilon <= std::min ( std::max ( y1, y2 ), std::max ( y3, y4 ) ) )
        {

            if ( lg.min_level() <= ues::log::TRACE_LVL )
            {
                ues::log::event e ( ues::log::TRACE_LVL, component_name, "Segment intersection found" );
                e.message() << "Intersection between segments ( " << s1p1 << ", " << s1p2 << " ) and ( " << s2p1 << ", " << s2p2 << " ) is " << result << ".\n";
                lg.record ( std::move ( e ) );
            }


            return true;
        }
    }

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Segment intersection not found" );
        e.message() << "No intersection found between segments ( " << s1p1 << ", " << s1p2 << " ) and ( " << s2p1 << ", " << s2p2 << " ).\n";
        if ( denominator != 0 )
        {
            e.message() << "Lines containing segments intersect at " << result << ", which is out of the boundaries of the segments.\n";
        }
        else
        {
            e.message() << "Segments are parallel.\n";
        }
        lg.record ( std::move ( e ) );
    }

    return false;
}


ues::math::numeric_type ues::geom::point_to_segment_distance ( const point<2> & origin_point,
                                                    const point<2> & segment_point1,
                                                    const point<2> & segment_point2,
                                                    const ues::math::numeric_type & angle,
                                                    const ues::math::numeric_type & epsilon )
{
    point<2> s1 = segment_point1;
    point<2> s2 = segment_point2;

    ues::math::matrix transform = rotation_matrix_2d ( -angle ) *
                       translation_matrix_2d ( -origin_point.get_x(), -origin_point.get_y() );
    s1.transform ( transform );
    s2.transform ( transform );

    if ( ( s1.get_y() > epsilon && s2.get_y() > epsilon ) ||
            ( s1.get_y() < -epsilon && s2.get_y() < -epsilon ) )
    {
        std::ostringstream out;
        out << "The angle provided (" << ( angle > ues::math::pi ? angle - 2 * ues::math::pi : angle ) << " rad) from point " << origin_point;
        out << " does not intersect segment " << segment_point1 << "-" << segment_point2;

        throw ues::exc::exception ( out.str(), UES_CONTEXT );
    }

    if ( std::abs ( s1.get_y() ) < ues::math::epsilon && std::abs ( s2.get_y() ) < ues::math::epsilon )
    {
        // The segment is perpendicular.
        return std::min ( s1.get_x(), s2.get_x() );
    }

    ues::math::numeric_type w1, w2;
    if ( s1.get_y() > 0 && s2.get_y() > 0 )
    {
        if ( s1.get_y() > s2.get_y() )
        {
            w1 = 0;
            w2 = 1;
        }
        else
        {
            w1 = 1;
            w2 = 0;
        }
    }
    else if ( s1.get_y() < 0 && s2.get_y() < 0 )
    {
        if ( s1.get_y() > s2.get_y() )
        {
            w1 = 1;
            w2 = 0;
        }
        else
        {
            w1 = 0;
            w2 = 1;
        }
    }
    else
    {
        ues::math::numeric_type inv_segment_height = 1 / std::abs ( s1.get_y() - s2.get_y() );
        w1 = std::abs ( s2.get_y() ) * inv_segment_height;
        w2 = std::abs ( s1.get_y() ) * inv_segment_height;
    }

    ues::math::numeric_type intersection_distance = w1 * s1.get_x() + w2 * s2.get_x();

    // Return distance from point to segment.
    return intersection_distance;
}


bool ues::geom::check_polygon_intersection ( const polygon & poly1, const polygon & poly2 ) noexcept
{
    return boost::geometry::intersects ( poly1, poly2 );
}


ues::math::matrix ues::geom::translation_matrix_2d ( const ues::math::numeric_type & x, const ues::math::numeric_type & y ) noexcept
{
    // A 3x3 matrix for homogeneous 2D transformation
    ues::math::matrix::fixed<3, 3> transform = { 1, 0, 0, 0, 1, 0, x, y, 1 };
    return transform;
}


ues::math::matrix ues::geom::rotation_matrix_2d ( const ues::math::numeric_type & angle ) noexcept
{

    // Compute the transformation matrix
    ues::math::numeric_type sine = std::sin ( angle );
    ues::math::numeric_type cosine = std::cos ( angle );

    // A 3x3 matrix for homogeneous 2D transformation
    ues::math::matrix::fixed<3, 3> transform = { cosine, sine, 0, -sine, cosine, 0, 0, 0, 1 };
    return transform;
}
