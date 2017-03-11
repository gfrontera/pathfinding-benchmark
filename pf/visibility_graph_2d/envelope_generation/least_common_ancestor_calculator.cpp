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

#include "least_common_ancestor_calculator.h"

#include <cassert>

using namespace ues::pf::vg2d;


least_common_ancestor_calculator::least_common_ancestor_calculator ( integer_type tree_size ) noexcept
:
least_significant_one ( tree_size ),
                      most_significant_one ( tree_size )
{
    // Linear time initialization.
    integer_type exponential = 1;
    integer_type power = 0;
    while ( exponential <= tree_size )
    {
        // Save information regarding the most significant bit.
        integer_type next_exponential = exponential << 1;
        for ( integer_type i = exponential; i <= tree_size && i < next_exponential; ++i )
        {
            most_significant_one[i - 1] = power;
        }

        for ( integer_type i = exponential; i <= tree_size; i += next_exponential )
        {
            least_significant_one[i - 1] = power;
        }

        // Create a mask with "power" ones to the right.
        mask.push_back ( exponential - 1 );

        exponential = next_exponential;
        ++power;
    }

    // Create a mask with "power" ones to the right.
    mask.push_back ( exponential - 1 );
}


least_common_ancestor_calculator::integer_type
least_common_ancestor_calculator::compute ( integer_type x, integer_type y ) const
{
    // Rightmost (least significant) one in x and y.
    const integer_type x_lso = least_significant_one[x - 1];
    const integer_type y_lso = least_significant_one[y - 1];

    // Leftmost (most significant) bit in which x and y differ.
    const integer_type most_significant_index = std::min ( x ^ y, static_cast< integer_type > ( most_significant_one.size() ) );
    const integer_type diff_xy = most_significant_one[ ( most_significant_index > 0 ) ? most_significant_index - 1 : 0 ];

    const integer_type n_of_bits = std::max ( diff_xy, std::max ( x_lso, y_lso ) );

    integer_type result;
    if ( n_of_bits == y_lso )
    {
        result = ~mask[n_of_bits + 1] & y;
    }
    else
    {
        result = ~mask[n_of_bits + 1] & x;
    }

    result |= mask[n_of_bits + 1] & ~mask[n_of_bits];

    // Two simple assertions to check the result is not
    // out of the range of the expected value.
    assert ( x <= result );
    assert ( result <= y );

    return result;
}
