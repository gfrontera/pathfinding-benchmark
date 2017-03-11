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

#ifndef UES_GEOM_POINT_H
#define UES_GEOM_POINT_H

#include "point_base.h"

namespace ues
{
namespace geom
{

/** This class represents a generic point in a space of N dimensions. */
template<unsigned short N>
class point : public point_base<N>
{
public:
    /** \name Constructor methods */
    /** \{ */

    inline point() noexcept = default;

    /** \} */

    /** \name Clone methods */
    /** \{ */

    inline point<N> * clone() const & noexcept;
    inline point<N> * clone() && noexcept;

    /** \} */

};

// Addition operator
template<unsigned short N>
point<N> operator+ ( const point<N> &, const ues::math::vector & ) noexcept;

/** Specialized point class for two-dimensional space. */
template<>
class point<2> : public point_base<2>
{
public:
    /** \name Constructor methods */
    /** \{ */

    inline point() noexcept = default;
    inline point ( const ues::math::numeric_type & x, const ues::math::numeric_type & y ) noexcept;

    /** \} */

    /** \name Clone methods */
    /** \{ */

    inline point<2> * clone() const & noexcept;
    inline point<2> * clone() && noexcept;

    /** \} */

    /** \name Getter and setter methods */

    /** \{ */

    inline const ues::math::numeric_type & get_x() const noexcept;
    inline const ues::math::numeric_type & get_y() const noexcept;
    inline void set_x ( const ues::math::numeric_type & ) noexcept;
    inline void set_y ( const ues::math::numeric_type & ) noexcept;

    /** \} */

    /** Returns angle (in radians) from this point to given point. */
    inline ues::math::numeric_type angle_to ( const point<2> & ) const noexcept;

};


/** Specialized point class for three-dimensional space. */
template<>
class point<3> : public point_base<3>
{
public:
    /** \name Constructor methods */
    /** \{ */

    inline point() noexcept = default;
    inline point ( const ues::math::numeric_type & x, const ues::math::numeric_type & y, const ues::math::numeric_type & z ) noexcept;

    /** \} */

    /** \name Clone methods */
    /** \{ */

    inline point * clone() const & noexcept;
    inline point * clone() && noexcept;

    /** \} */

    /** \name Getter and setter methods */
    /** \{ */

    inline const ues::math::numeric_type & get_x() const noexcept;
    inline const ues::math::numeric_type & get_y() const noexcept;
    inline const ues::math::numeric_type & get_z() const noexcept;
    inline void set_x ( const ues::math::numeric_type & ) noexcept;
    inline void set_y ( const ues::math::numeric_type & ) noexcept;
    inline void set_z ( const ues::math::numeric_type & ) noexcept;

    /** \} */

};


// Template implementation for the general case.


template<unsigned short N>
point<N> operator+ ( const point<N> & base, const ues::math::vector & offset ) noexcept
{
    point<N> result = base;
    result += offset;
    return result;
}


// Template implementation for the two-dimensional case.


point<2>::point ( const ues::math::numeric_type & x, const ues::math::numeric_type & y ) noexcept
    : point_base ( std::make_unique<ues::math::vector> ( ues::math::vector::fixed<3> ( { x, y, 1 } ) ) )
{
}


point<2> * point<2>::clone() const & noexcept
{
    return new point<2> ( *this );
}


point<2> * point<2>::clone() && noexcept
{
    return new point<2> ( std::move ( *this ) );
}


const ues::math::numeric_type & point<2>::get_x() const noexcept
{
    return coordinates->at ( 0 );
}


const ues::math::numeric_type & point<2>::get_y() const noexcept
{
    return coordinates->at ( 1 );
}


void point<2>::set_x ( const ues::math::numeric_type & value ) noexcept
{
    coordinates->at ( 0 ) = value;
}


void point<2>::set_y ( const ues::math::numeric_type & value ) noexcept
{
    coordinates->at ( 1 ) = value;
}


ues::math::numeric_type point<2>::angle_to ( const point<2> & other ) const noexcept
{
    return std::atan2 ( other.get_y() - get_y(), other.get_x() - get_x() );
}


// Template implementation for the three-dimensional case.


point<3>::point ( const ues::math::numeric_type & x, const ues::math::numeric_type & y, const ues::math::numeric_type & z ) noexcept
:
point_base ( std::make_unique<ues::math::vector> ( ues::math::vector::fixed<4> ( { x, y, z, 1 } ) ) )
{
}


point<3> * point<3>::clone() const & noexcept
{
    return new point<3> ( *this );
}


point<3> * point<3>::clone() && noexcept
{
    return new point<3> ( std::move ( *this ) );
}


const ues::math::numeric_type & point<3>::get_x() const noexcept
{
    return coordinates->at ( 0 );
}


const ues::math::numeric_type & point<3>::get_y() const noexcept
{
    return coordinates->at ( 1 );
}


const ues::math::numeric_type & point<3>::get_z() const noexcept
{
    return coordinates->at ( 2 );
}


void point<3>::set_x ( const ues::math::numeric_type & value ) noexcept
{
    coordinates->at ( 0 ) = value;
}


void point<3>::set_y ( const ues::math::numeric_type & value ) noexcept
{
    coordinates->at ( 1 ) = value;
}


void point<3>::set_z ( const ues::math::numeric_type & value ) noexcept
{
    coordinates->at ( 2 ) = value;
}


}
}


namespace std
{

/** Hash function for objects of type ues::geom::point<N>. */
template <unsigned short N>
struct hash<ues::geom::point<N>>
{
    inline size_t operator() ( const ues::geom::point<N> & ) const noexcept;
};


// Template implementation.


template <unsigned short N>
size_t hash< ues::geom::point<N> >::operator() ( const ues::geom::point<N> & p ) const noexcept
{
    hash< ues::math::numeric_type > hasher;
    size_t result = hasher ( p.get ( 0 ) );
    for ( unsigned int i = 1; i < N; ++i )
    {
        result ^= hasher ( p.get ( i ) );
    }
    return result;
}


}

#endif // UES_GEOM_POINT_H
