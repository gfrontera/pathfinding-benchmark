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

#ifndef UES_GEOM_POINT_BASE_H
#define UES_GEOM_POINT_BASE_H

#include <memory>

#include <armadillo>

#include <math/algebra.h>
#include <math/constants.h>

#include "exc/exception.h"

namespace ues
{
namespace geom
{

template<unsigned short N>
class point_base
{
public:

    /** \name Constructor and destructor methods */
    /** \{ */

    /** Default constructor method. */
    point_base() noexcept;
    /** Copy constructor method. */
    point_base ( const point_base<N> & ) noexcept;
    /** Move constructor method. */
    point_base ( point_base<N> && ) noexcept;

    /** Destructor method. */
    virtual ~point_base() = default;
    /** \} */

    /** \name Clone methods */
    /** \{ */

    /** Returns a pointer to a copy of this point. */
    virtual point_base<N> * clone() const & noexcept = 0;
    /** Returns a pointer to a copy of this point, destroying the original. */
    virtual point_base<N> * clone() && noexcept = 0;

    /** \} */

    /** \name Getter and setter methods */
    /** \{ */

    inline const ues::math::numeric_type & get ( unsigned short i ) const;
    inline void set ( unsigned short i, const ues::math::numeric_type & );

    /** \} */

    /** \name Operators */
    /** \{ */

    point_base<N> & operator= ( const point_base<N> & ) noexcept;
    point_base<N> & operator= ( point_base<N> && ) noexcept;

    bool operator== ( const point_base<N> & other ) const noexcept;
    bool operator!= ( const point_base<N> & other ) const noexcept;

    point_base<N> & operator+= ( const ues::math::vector & ) noexcept;

    /** \} */

    /** Returns \c true if this point and \a other are the same point, with the specified precision. */
    bool equal ( const point_base<N> & other, const ues::math::numeric_type & epsilon = ues::math::epsilon ) const noexcept;

    /** Outputs a textual description of the point to a ostream object. */
    void describe ( std::ostream & out ) const noexcept;

    /** Applies a transformation matrix to the point. */
    point_base<N> & transform ( const ues::math::matrix & transformation_matrix );

    /** Returns distance from this point to given point. */
    ues::math::numeric_type distance_to ( const point_base<N> & other ) const noexcept;

    template<unsigned short M>
    friend ues::math::vector operator- ( const point_base<M> & p1, const point_base<M> & p2 ) noexcept;

protected:
    /** Constructor method. */
    point_base ( std::unique_ptr< ues::math::vector > coordinates ) noexcept;

    /** Transforms point to homogeneous coordinates. */
    void homogenize();

    std::unique_ptr< ues::math::vector > coordinates;
};

template<unsigned short N>
point_base<N> * new_clone ( const point_base<N> & other ) noexcept;

template<unsigned short N>
point_base<N> * new_clone ( point_base<N> && other ) noexcept;

/** Output stream operator. */
template<unsigned short N>
std::ostream & operator<< ( std::ostream & out, const ues::geom::point_base<N> & ) noexcept;

/** Difference operator. */
template<unsigned short N>
ues::math::vector operator- ( const point_base<N> & p1, const point_base<N> & p2 ) noexcept;


// Template implementation


template<unsigned short N>
point_base<N>::point_base() noexcept
    : coordinates ( std::make_unique < ues::math::vector::fixed < N + 1 > > ( arma::fill::zeros ) )
{
    coordinates->at ( N ) = 1;
}

template<unsigned short N>
point_base<N>::point_base ( const point_base<N> & other ) noexcept
    : coordinates ( std::make_unique < ues::math::vector::fixed < N + 1 > > ( *other.coordinates ) )
{
}


template<unsigned short N>
point_base<N>::point_base ( point_base<N> && other ) noexcept
    : coordinates ( std::unique_ptr<ues::math::vector> ( other.coordinates.release() ) )
{
}


template<unsigned short N>
point_base<N>::point_base ( std::unique_ptr< ues::math::vector > coordinates ) noexcept
    :
    coordinates ( std::move ( coordinates ) )
{
}


template<unsigned short N>
point_base<N> & point_base<N>::operator= ( const point_base<N> & other ) noexcept
{
    coordinates.reset ( new ues::math::vector::fixed < N + 1 > ( *other.coordinates ) );
    return *this;
}


template<unsigned short N>
point_base<N> & point_base<N>::operator= ( point_base<N> && other ) noexcept
{
    coordinates.reset ( other.coordinates.release() );
    return *this;
}


template<unsigned short N>
const math::numeric_type & point_base<N>::get ( short unsigned int i ) const
{
    return coordinates->operator() ( i );
}


template<unsigned short N>
void point_base<N>::set ( short unsigned int i, const math::numeric_type & value )
{
    coordinates->operator() ( i ) = value;
}


template<unsigned short N>
bool point_base<N>::operator== ( const point_base<N> & other ) const noexcept
{
    return arma::accu ( *this->coordinates != *other.coordinates ) == 0;
}


template<unsigned short N>
bool point_base<N>::operator!= ( const point_base<N> & other ) const noexcept
{
    return ! ( *this == other );
}


template<unsigned short N>
point_base< N > & point_base<N>::operator+= ( const ues::math::vector & offset ) noexcept
{
    *this->coordinates += offset;
    return *this;
}


template<unsigned short N>
bool point_base<N>::equal ( const point_base<N> & other, const ues::math::numeric_type & epsilon ) const noexcept
{
    return arma::accu ( arma::abs ( *this->coordinates - *other.coordinates ) ) < epsilon;
}


template<unsigned short N>
void point_base<N>::describe ( std::ostream & out ) const noexcept
{
    out << "(" << coordinates->at ( 0 );

    for ( unsigned int i = 1; i < coordinates->n_rows - 1; i++ )
    {
        out << ", " << coordinates->at ( i );
    }

    out << ")";
}


template<unsigned short N>
point_base<N> & point_base<N>::transform ( const ues::math::matrix & m )
{
    *coordinates = m * ( *coordinates );
    homogenize();
    return *this;
}


template<unsigned short N>
void point_base<N>::homogenize()
{
    if ( coordinates->at ( coordinates->n_rows - 1 ) != 1 )
    {
        if ( coordinates->at ( coordinates->n_rows - 1 ) == 0 )
        {
            throw ues::exc::exception ( "Cannot homogenize coordinates for this vector", UES_CONTEXT );
        }
        *coordinates /= coordinates->at ( coordinates->n_rows - 1 );
    }
}


template<unsigned short N>
ues::math::numeric_type point_base<N>::distance_to ( const point_base<N> & other ) const noexcept
{
    return arma::norm ( *other.coordinates - *coordinates, 2 );
}


template<unsigned short N>
point_base<N> * new_clone ( const point_base<N> & other ) noexcept
{
    return other.clone();
}


template<unsigned short N>
point_base<N> * new_clone ( point_base<N> && other ) noexcept
{
    return std::move ( other ).clone();
}


template<unsigned short N>
std::ostream & operator<< ( std::ostream & out, const point_base<N> & p ) noexcept
{
    p.describe ( out );
    return out;
}


template<unsigned short N>
ues::math::vector operator- ( const point_base<N> & p1, const point_base<N> & p2 ) noexcept
{
    return *p1.coordinates - * p2.coordinates;
}


}
}

#endif // UES_GEOM_POINT_BASE_H
