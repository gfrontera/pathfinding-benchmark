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

#include <pf/visibility_graph_2d/point_sort/point_sorter.h>


TEST ( pf, point_sorter )
{
    std::shared_ptr< ues::pf::vg2d::point_vector > points = std::make_shared< ues::pf::vg2d::point_vector >();
    points->push_back ( ues::geom::point<2> ( 0, 0 ) );
    points->push_back ( ues::geom::point<2> ( 0, 1 ) );
    points->push_back ( ues::geom::point<2> ( -1, 0 ) );
    points->push_back ( ues::geom::point<2> ( 0, -1 ) );
    points->push_back ( ues::geom::point<2> ( 1, 0 ) );

    ues::pf::vg2d::point_sorter ps ( points );
    ues::pf::vg2d::point_index_vector result = ps.get_sorted_list_of_points ( 0 );

    ues::pf::vg2d::point_index_vector expected_result = { 1, 2, 3, 4 };

    ASSERT_EQ ( expected_result, result );
}
