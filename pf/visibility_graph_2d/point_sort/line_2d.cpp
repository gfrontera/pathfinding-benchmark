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

#include "line_2d.h"

using namespace ues::pf::vg2d;


line_2d::line_2d ( const ues::math::numeric_type & x1, const ues::math::numeric_type & x0 ) noexcept
    : parameters ( std::make_unique<ues::math::vector> ( ues::math::vector::fixed<2> ( { x1, x0 } ) ) )
{
}


ues::math::numeric_type line_2d::evaluate ( const ues::math::numeric_type & x ) const noexcept
{
    return x * get_x1() + get_x0();
}


std::ostream & ues::pf::vg2d::operator<< ( std::ostream & out, const line_2d & l ) noexcept
{
    return out << "y = " << l.get_x1() << " x + " << l.get_x0();
}
