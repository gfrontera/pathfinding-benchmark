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

#ifndef UES_TESTS_COMPARE_PATHS
#define UES_TESTS_COMPARE_PATHS

#include "gtest/gtest.h"

#include <pf/path.h>

namespace
{

void compare_paths ( const ues::pf::path<3> & expected, const ues::pf::path<3> & real )
{
    ASSERT_EQ ( expected.size(), real.size() ) << "Resulting path does not have the expected size.";
    for ( typename ues::pf::path<3>::size_type i = 0; i < expected.size(); ++i )
    {
        const ues::geom::point<3> & expected_point = dynamic_cast<const ues::geom::point<3> &> ( expected[i] );
        const ues::geom::point<3> & real_point = dynamic_cast<const ues::geom::point<3> &> ( real[i] );
        ASSERT_DOUBLE_EQ ( expected_point.get_x(), real_point.get_x() ) << "Resulting path has unexpected coordinate.";
        ASSERT_DOUBLE_EQ ( expected_point.get_y(), real_point.get_y() ) << "Resulting path has unexpected coordinate.";
        ASSERT_DOUBLE_EQ ( expected_point.get_z(), real_point.get_z() ) << "Resulting path has unexpected coordinate.";
    }
}

}

#endif // UES_TESTS_COMPARE_PATHS
