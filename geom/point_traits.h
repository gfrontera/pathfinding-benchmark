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

#ifndef UES_GEOM_POINT_TRAITS_H
#define UES_GEOM_POINT_TRAITS_H

#include <boost/geometry/core/tag.hpp>
#include <boost/geometry/core/coordinate_type.hpp>
#include <boost/geometry/core/coordinate_system.hpp>
#include <boost/geometry/core/coordinate_dimension.hpp>
#include <boost/geometry/core/access.hpp>
#include <boost/geometry/core/cs.hpp>

#include "point.h"

// Traits

namespace boost
{
namespace geometry
{
namespace traits
{

template<unsigned short N>
struct tag< ues::geom::point<N> >
{
    typedef point_tag type;
};

template<unsigned short N>
struct coordinate_type< ues::geom::point<N> >
{
    typedef ues::math::numeric_type type;
};

template<unsigned short N>
struct coordinate_system< ues::geom::point<N> >
{
    typedef cs::cartesian type;
};

template<unsigned short N>
struct dimension< ues::geom::point<N> > : boost::mpl::int_<N>
{
};

template<unsigned short N, unsigned long I>
struct access< ues::geom::point<N>, I >
{
    static ues::math::numeric_type get ( ues::geom::point<N> const & p )
    {
        return p.get ( I );
    }

    static void set ( ues::geom::point<N> & p, ues::math::numeric_type const & value )
    {
        p.set ( I, value );
    }
};

}
}
}

#endif // UES_GEOM_POINT_TRAITS_H
