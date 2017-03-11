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

#ifndef UES_GEOM_POLYGON_H
#define UES_GEOM_POLYGON_H

#include "point.h"
#include "segment.h"
#include "box_2d.h"

namespace ues
{
namespace geom
{

class polygon
{
public:
    typedef std::vector< point<2> > point_vector;
    typedef point_vector::size_type size_type;

    /** Constructor method from a point vector. */
    polygon ( point_vector points );
    /** Constructor method from an initializer list. */
    polygon ( std::initializer_list< point<2> > il );


    /** Outputs a textual description of the polygon to a ostream object. */
    void describe ( std::ostream & out ) const noexcept;

    /** Checks whether the \a input_segment intersects the polygon. If there is an intersection,
     * this method returns \c true and the \a intersection_point nearest to the first point of the
     * segment. */
    bool check_intersection ( const segment<2> & input_segment, point<2> & intersection_point ) const noexcept;

    /** Checks whether the \a input_polygon intersects the polygon. If there is an intersection,
     * this method returns \c true. */
    bool check_intersection ( const polygon & input_polygon ) const noexcept;

    /** Checks whether an \a input_point is inside the polygon. */
    bool is_inside ( const point<2> & input_point ) const noexcept;

    /** Returns the number of vertices in the polygon. */
    inline size_type size() const noexcept;

    /** Applies a transformation matrix to all the vertices of the polygon. */
    polygon & transform ( const ues::math::matrix & transformation_matrix );

    /** \name Operators */
    /** \{ */

    bool operator== ( const polygon & ) const noexcept;
    bool operator!= ( const polygon & ) const noexcept;

    /** \} */

    /** \name Getter and setter methods */
    /** \{ */

    inline const point<2> & get_point_at ( point_vector::size_type ) const noexcept;

    /** \} */

    /** \name Iterator access */
    /** \{ */

    inline point_vector::iterator begin() noexcept;
    inline point_vector::iterator end() noexcept;
    inline point_vector::reverse_iterator rbegin() noexcept;
    inline point_vector::reverse_iterator rend() noexcept;

    inline point_vector::const_iterator begin() const noexcept;
    inline point_vector::const_iterator end() const noexcept;
    inline point_vector::const_reverse_iterator rbegin() const noexcept;
    inline point_vector::const_reverse_iterator rend() const noexcept;

    /** \} */

private:
    /** A bounding box that contains the whole polygon. It can be used to check efficiently
     * if a point has a chance of being inside the polygon. */
    box_2d bounding_box;

    /** The sequence of points that conform the vertices of the polygon. */
    point_vector points;

    /** Checks whether the \a input_segment intersects the polygon. It returns the number
     * of times the segment intersects the boundaries of the polygon and
     * the \a intersection_point nearest to the first point of the segment. */
    point_vector::size_type get_number_of_intersections ( const segment<2> & input_segment, point<2> & intersection_point ) const noexcept;

    /** Checks whether the \a input_segment intersects one of the sides of the polygon. */
    bool check_polygon_side_intersection ( const segment<2> & input_segment,
                                           const point<2> & first_side_point,
                                           const point<2> & second_side_point,
                                           point<2> & intersection_point ) const noexcept;

    /** Updates the bounding box with the content of \c points. */
    void update_bounding_box() noexcept;
};

/** Output stream operator. */
std::ostream & operator<< ( std::ostream & out, const ues::geom::polygon & );

// Inlined methods.

ues::geom::polygon::size_type polygon::size() const noexcept
{
    return points.size();
}


const point<2> & polygon::get_point_at ( point_vector::size_type i ) const noexcept
{
    return points.at ( i );
}


polygon::point_vector::const_iterator polygon::begin() const noexcept
{
    return points.begin();
}


polygon::point_vector::const_iterator polygon::end() const noexcept
{
    return points.end();
}


polygon::point_vector::const_reverse_iterator polygon::rbegin() const noexcept
{
    return points.rbegin();
}


polygon::point_vector::const_reverse_iterator polygon::rend() const noexcept
{
    return points.rend();
}


polygon::point_vector::iterator polygon::begin() noexcept
{
    return points.begin();
}


polygon::point_vector::iterator polygon::end() noexcept
{
    return points.end();
}


polygon::point_vector::reverse_iterator polygon::rbegin() noexcept
{
    return points.rbegin();
}


polygon::point_vector::reverse_iterator polygon::rend() noexcept
{
    return points.rend();
}

}
}

#endif // UES_GEOM_POLYGON_H
