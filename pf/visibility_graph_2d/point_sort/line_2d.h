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

#ifndef UES_PF_VG2D_LINE_2D_H
#define UES_PF_VG2D_LINE_2D_H

#include <memory>

#include <math/algebra.h>

namespace ues
{
namespace pf
{
namespace vg2d
{
    
class line_2d
{
public:
    /** Creates a line defined by "y = x1*x + x0". */
    line_2d ( const ues::math::numeric_type & x1,  const ues::math::numeric_type & x0 ) noexcept;

    inline ues::math::numeric_type get_x1() const noexcept;
    inline ues::math::numeric_type get_x0() const noexcept;

    ues::math::numeric_type evaluate ( const ues::math::numeric_type & x ) const noexcept;

private:
    std::unique_ptr< ues::math::vector > parameters;
};

/** Output stream operator. */
std::ostream & operator<< ( std::ostream &, const line_2d & ) noexcept;

// Inlined methods

ues::math::numeric_type line_2d::get_x1() const noexcept
{
    return parameters->at ( 0 );
}

ues::math::numeric_type line_2d::get_x0() const noexcept
{
    return parameters->at ( 1 );
}

}
}
}

#endif // UES_PF_VG2D_LINE_2D_H
