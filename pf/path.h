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

#ifndef UES_PF_PATH_H
#define UES_PF_PATH_H

#include <vector>

#include <geom/point.h>

namespace ues
{
namespace pf
{

template<unsigned short N>
class path : public std::vector< ues::geom::point<N> >
{
public:
    using std::vector< ues::geom::point<N> >::vector;

public:
    /** Returns the length of the path. */
    inline ues::math::numeric_type length() const noexcept;

};

/** Output stream operator for a path. */
template<unsigned short N>
inline std::ostream & operator<< ( std::ostream & out, const path<N> & pt );


// Template implementation.


template<unsigned short N>
ues::math::numeric_type path<N>::length() const noexcept
{
    ues::math::numeric_type result = 0;
    for ( typename std::vector< ues::geom::point<N> >::size_type i = 1; i < this->size(); ++i )
    {
        result += this->at ( i - 1 ).distance_to ( this->at ( i ) );
    }
    return result;
}


template<unsigned short N>
std::ostream & operator<< ( std::ostream & out, const path<N> & pt )
{
    out << "{ ";
    if ( !pt.empty() )
    {
        out << pt.front();
    }
    for ( typename path<N>::size_type i = 1; i < pt.size(); ++i )
    {
        out << ", " << pt[i];
    }
    out << " }";
    return out;
}


}
}

#endif // UES_PF_PATH_H
