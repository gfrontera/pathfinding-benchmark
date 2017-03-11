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

#include "gtest/gtest.h"

#include <pf/visibility_graph_2d/envelope_generation/least_common_ancestor_calculator.h>


TEST ( pf, least_common_ancestor_calculator )
{
    using ues::pf::vg2d::least_common_ancestor_calculator;

    {
        least_common_ancestor_calculator lca ( 17 );

        ASSERT_EQ ( 8, lca.compute ( 2, 15 ) );
        ASSERT_EQ ( 12, lca.compute ( 9, 15 ) );
        ASSERT_EQ ( 1, lca.compute ( 1, 1 ) );
        ASSERT_EQ ( 2, lca.compute ( 1, 2 ) );
        ASSERT_EQ ( 16, lca.compute ( 1, 16 ) );
    }
    
    {
        least_common_ancestor_calculator lca ( 256 );

        ASSERT_EQ ( 128, lca.compute ( 127, 129 ) );
        ASSERT_EQ ( 148, lca.compute ( 145, 151 ) );
        ASSERT_EQ ( 64, lca.compute ( 64, 72 ) );
        ASSERT_EQ ( 256, lca.compute ( 1, 256 ) );
    }
}
