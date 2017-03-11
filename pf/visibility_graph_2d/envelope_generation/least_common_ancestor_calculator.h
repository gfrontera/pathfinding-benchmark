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

#ifndef UES_PF_VG2D_LEAST_COMMON_ANCESTOR_CALCULATOR_H
#define UES_PF_VG2D_LEAST_COMMON_ANCESTOR_CALCULATOR_H

#include <exception>
#include <vector>

namespace ues
{
namespace pf
{
namespace vg2d
{

class least_common_ancestor_calculator
{
public:
    typedef unsigned int integer_type;

    least_common_ancestor_calculator ( integer_type size ) noexcept;

    integer_type compute ( integer_type, integer_type ) const;

private:
    std::vector< integer_type > least_significant_one;
    std::vector< integer_type > most_significant_one;
    std::vector< integer_type > mask;
};

}
}
}

#endif // UES_PF_VG2D_LEAST_COMMON_ANCESTOR_CALCULATOR_H
