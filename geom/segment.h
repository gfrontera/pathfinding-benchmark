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

#ifndef UES_GEOM_SEGMENT_H
#define UES_GEOM_SEGMENT_H

#include <utility>
#include <iostream>

#include "point.h"

namespace ues
{
namespace geom
{

template<unsigned short N>
class segment
{
public:
    /** Constructor methods. */
    /** \{ */

    /** Default constructor method. */
    segment() noexcept = default;
    /** Constructor method. */
    segment ( point<N> point_first, point<N> point_second ) noexcept;

    /** \} */

    /** \name Getter methods */
    /** \{ */

    /** Returns the first point of the segment. */
    const point<N> & get_point_first() const noexcept;
    /** Returns the second point of the segment. */
    const point<N> & get_point_second() const noexcept;

    /** \} */

    /** Computes the length of the segment. */
    ues::math::numeric_type length() const noexcept;

private:

    point<N> point_first, point_second;
};

/** Output stream operator. */
template<unsigned short N>
std::ostream & operator<< ( std::ostream & out, const segment<N> & seg ) noexcept;


// Template implementation.


template<unsigned short N>
segment<N>::segment ( point<N> point_first, point<N> point_second ) noexcept
    : point_first ( std::move ( point_first ) ),
      point_second ( std::move ( point_second ) )
{
}


template<unsigned short N>
const point<N> & segment<N>::get_point_first() const noexcept
{
    return point_first;
}


template<unsigned short N>
const point<N> & segment<N>::get_point_second() const noexcept
{
    return point_second;
}


template<unsigned short N>
ues::math::numeric_type segment<N>::length() const noexcept
{
    return point_first.distance_to ( point_second );
}


template<unsigned short N>
std::ostream & operator<< ( std::ostream & out, const segment<N> & seg ) noexcept
{
    return out << "( " << seg.get_point_first() << "-" << seg.get_point_second() << " )";
}


}
}

#endif // UES_GEOM_SEGMENT_H
