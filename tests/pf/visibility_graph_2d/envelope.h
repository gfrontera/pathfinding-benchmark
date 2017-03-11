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

#include <pf/visibility_graph_2d/envelope_generation/envelope.h>


TEST ( pf, envelope )
{
    using namespace ues::pf::vg2d;

    std::vector< envelope::segment_data > sd = { { 0, 2 }, { 1, 3 }, { 2, 3 } };

    envelope env ( 4 );
    ASSERT_TRUE ( env.empty() ) << "Envelope is not created empty.";

    envelope::iterator it = env.shortest_as_long_as ( sd[0] );
    env.insert ( sd[0], it );
    ASSERT_FALSE ( env.empty() )  << "Envelope reported empty after inserting.";
    ASSERT_EQ ( *env.head(), sd[0] ) << "Incorrect content after first insertion.";

    it = env.shortest_as_long_as ( sd[1] );
    env.insert ( sd[1], it );
    ASSERT_EQ ( *env.head(), sd[0] ) << "Incorrect content after second insertion.";

    it = env.shortest_as_long_as ( sd[2] );
    envelope::iterator it2 = env.pred ( it );
    ASSERT_EQ ( *it, sd[1] ) << "Function shortest_as_long_as returns incorrect iterator.";
    ASSERT_EQ ( *it2, sd[0] ) << "Function pred returns incorrect iterator.";

    env.erase ( it );
    env.insert ( sd[2], it2 );
    env.erase ( it2 );
    ASSERT_EQ ( *env.head(), sd[2] ) << "Incorrect content after third insertion.";
}
