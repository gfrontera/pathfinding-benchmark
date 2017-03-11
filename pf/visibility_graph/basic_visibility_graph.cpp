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

#include "basic_visibility_graph.h"

using namespace ues::pf;


template<unsigned short N>
basic_visibility_graph<N>::basic_visibility_graph() noexcept
    : basic_visibility_graph ( 0 )
{
}


template<unsigned short N>
basic_visibility_graph<N>::basic_visibility_graph ( size_type number_of_points ) noexcept
    : number_of_points ( number_of_points ),
      vv ( ( number_of_points == 0 ) ? number_of_points : number_of_points - 1 )
{
}


template<unsigned short N>
basic_visibility_graph<N>::~basic_visibility_graph() noexcept
{
}


template<unsigned short N>
bool basic_visibility_graph<N>::check_visibility ( size_type point1_index,
                                                   size_type point2_index,
                                                   ues::math::numeric_type & distance ) const
{
    // If both points are the same, the result is trivial.
    if ( point1_index == point2_index )
    {
        distance = 0;
        return true;
    }

    // Search for distance information.
    size_type min_point_index = std::min ( point1_index, point2_index );
    size_type max_point_index = std::max ( point1_index, point2_index );

    const point_visibility & pv = vv[min_point_index];
    typename basic_visibility_graph<N>::point_visibility::const_iterator distance_info = pv.find ( max_point_index );
    if ( distance_info != pv.end() )
    {
        // If distance information is found, then both points are visible
        // from each other.
        distance = distance_info->second;
        return true;
    }
    else
    {
        // If there is no distance information, points are not visible from
        // each other.
        return false;
    }
}


template<unsigned short N>
void basic_visibility_graph<N>::add_visibility ( const ues::geom::point<N> & point1,
                                                 const ues::geom::point<N> & point2 )
{
    add_visibility ( point1, point2, point1.distance_to ( point2 ) );
}


template<unsigned short N>
void basic_visibility_graph<N>::add_visibility ( const ues::geom::point<N> & point1,
                                                 const ues::geom::point<N> & point2,
                                                 ues::math::numeric_type distance )
{
    size_type point1_index = point_to_index ( point1 );
    size_type point2_index = point_to_index ( point2 );

    add_visibility ( point1_index, point2_index, distance );
}


template<unsigned short N>
void basic_visibility_graph<N>::add_visibility ( size_type point1_index,
                                                 size_type point2_index,
                                                 ues::math::numeric_type distance )
{
    // Visibility information for a point with itself is trivial and it
    // is not added.
    if ( point1_index != point2_index )
    {
        size_type min_point_index = std::min ( point1_index, point2_index );
        size_type max_point_index = std::max ( point1_index, point2_index );

        point_visibility & pv = vv[min_point_index];
        pv[ max_point_index ] = distance ;
    }
}


template<unsigned short N>
void basic_visibility_graph<N>::add_point()
{
    // Increase the number of points and expand the
    // visibility vector.
    ++number_of_points;
    vv.resize ( number_of_points - 1 );
}


template<unsigned short N>
typename basic_visibility_graph<N>::size_type basic_visibility_graph<N>::size() const noexcept
{
    return number_of_points;
}


template<unsigned short N>
void basic_visibility_graph<N>::describe ( std::ostream & out ) const noexcept
{
    // Print all points in the graph.
    out << "Points:\tIndex\tCoordinates\n";
    for ( size_type i = 0; i < number_of_points; ++i )
    {
        out << '\t' << i << '\t' << index_to_point ( i ) << '\n';
    }
    // Print a first row with the names of the points
    out << "Distances:" << '\n' << '\t';
    for ( size_type i = 0; i < number_of_points; ++i )
    {
        out << i << '\t';
    }
    out << '\n';

    for ( size_type i = 0; i < number_of_points; ++i )
    {
        out << i << '\t';
        for ( size_type j = 0; j < number_of_points; ++j )
        {
            ues::math::numeric_type distance;
            if ( check_visibility ( i, j, distance ) )
            {
                out << distance;
            }
            else
            {
                out << '-';
            }
            out << '\t';
        }
        out << '\n';
    }
}


template<unsigned short N>
typename basic_visibility_graph<N>::point_index_vector basic_visibility_graph<N>::adjacents ( size_type point_index ) const
{
    point_index_vector result;

    // Adjacent points with index i < point_index are stored at
    // the visibility_info object corresponding to i (vv[i]).
    for ( size_type i = 0; i < point_index; ++i )
    {
        const point_visibility & pv = vv[i];
        typename point_visibility::const_iterator it = pv.find ( point_index );
        if ( it != pv.end() )
        {
            result.push_back ( i );
        }
    }

    // Adjacent points with index i > point_index are stored at the
    // visibility_info object corresponding to point_index (vv[point_index]).
    // Note that if point_index == number_of_points - 1, there is no
    // i > point_index and therefore all the adjacent points have already
    // been found.
    if ( point_index < number_of_points - 1 )
    {
        for ( auto point_visibility_info : vv[point_index] )
        {
            result.push_back ( point_visibility_info.first );
        }
    }

    return result;
}


// Instantiate the templates in this translation unit, just once.
template class ues::pf::basic_visibility_graph<2>;
template class ues::pf::basic_visibility_graph<3>;
