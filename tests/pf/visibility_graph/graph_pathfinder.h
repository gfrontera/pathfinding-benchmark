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

#include <pf/visibility_graph/graph_pathfinder.h>
#include <pf/visibility_graph_2d/visibility_graph.h>


TEST ( pf, pathfinder_2d )
{
    ues::pf::vg2d::shared_point_vector points = std::make_shared<const ues::pf::vg2d::point_vector> ( ues::pf::vg2d::point_vector { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } } );
    std::shared_ptr< ues::pf::vg2d::visibility_graph > vg = std::make_shared< ues::pf::vg2d::visibility_graph > ( points );

    vg->add_visibility ( (*points)[0], (*points)[1] );
    vg->add_visibility ( (*points)[0], (*points)[4] );
    vg->add_visibility ( (*points)[2], (*points)[5] );
    vg->add_visibility ( (*points)[3], (*points)[4] );
    vg->add_visibility ( (*points)[4], (*points)[5] );

    ues::pf::graph_pathfinder<2> finder ( vg );

    ues::pf::path<2> result = finder.find_path ( (*points)[0], (*points)[2] );

    // FIXME Temporary implementation until https://svn.boost.org/trac/boost/ticket/9330 is solved.
    ues::pf::path<2> expected_result;
    expected_result.push_back ( (*points)[0] );
    expected_result.push_back ( (*points)[4] );
    expected_result.push_back ( (*points)[5] );
    expected_result.push_back ( (*points)[2] );

    EXPECT_EQ ( expected_result, result );

}
