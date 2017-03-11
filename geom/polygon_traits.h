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

#ifndef UES_GEOM_POLYGON_TRAITS_H
#define UES_GEOM_POLYGON_TRAITS_H

#include <boost/geometry/core/tag.hpp>
#include <boost/geometry/core/closure.hpp>
#include <boost/geometry/core/point_order.hpp>

#include "point_traits.h"
#include "polygon.h"

// Traits

namespace boost
{
namespace geometry
{
namespace traits
{

template<>
struct tag<ues::geom::polygon>
{
    typedef ring_tag type;
};

template<>
struct point_order<ues::geom::polygon>
{
    static const order_selector value = clockwise;
};

template<>
struct closure<ues::geom::polygon>
{
    static const closure_selector value = open;
};

}
}

template<>
struct range_iterator<ues::geom::polygon>
{
    typedef ues::geom::polygon::point_vector::iterator type;
};

template<>
struct range_const_iterator<ues::geom::polygon>
{
    typedef ues::geom::polygon::point_vector::const_iterator type;
};

}

#endif
