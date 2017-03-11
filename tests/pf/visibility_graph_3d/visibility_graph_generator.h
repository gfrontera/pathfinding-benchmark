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

#include <env/environment.h>
#include <pf/visibility_graph_3d/visibility_graph_generator.h>
#include <pf/visibility_graph/graph_pathfinder.h>

#include <tests/pf/compare_paths.h>


TEST ( pf, visibility_graph_generator_3d_simple )
{
    using ues::pf::graph_pathfinder;

    ues::env::obstacle_vector obstacles;
    obstacles.push_back ( ues::env::obstacle ( { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } }, 1 ) );

    ues::geom::point<3> origin = { -1, .3, 0 };
    ues::geom::point<3> target = { 2, .3, 0 };

    ues::pf::graph_pathfinder<3> finder ( ues::pf::vg3d::visibility_graph_generator::generate_visibility_graph ( obstacles, origin, target ) );

    ues::pf::path<3> result = finder.find_path ( origin, target );
    ues::pf::path<3> expected_result;
    // FIXME Temporary implementation until https://svn.boost.org/trac/boost/ticket/9330 is solved.
    expected_result.push_back ( origin );
    expected_result.push_back ( ues::geom::point<3> ( 0, 0, 0 ) );
    expected_result.push_back ( ues::geom::point<3> ( 1, 0, 0 ) );
    expected_result.push_back ( target );

    compare_paths ( expected_result, result );

}


TEST ( pf, visibility_graph_generator_3d_multiple )
{
    using ues::pf::graph_pathfinder;

    ues::env::obstacle_vector obstacles;
    obstacles.push_back ( ues::env::obstacle ( { { -5, 0 }, { -4, 1 }, { -3, 0 }, { -4, -1 } }, 2 ) );
    obstacles.push_back ( ues::env::obstacle ( { { -1, 2 }, { 1, 2 }, { 1, -2 }, { -1, -2 } }, 1 ) );

    ues::geom::point<3> origin = { -7, 0, 0 };
    ues::geom::point<3> target = { 3, 1, 0 };

    ues::pf::graph_pathfinder<3> finder ( ues::pf::vg3d::visibility_graph_generator::generate_visibility_graph ( obstacles, origin, target ) );

    ues::pf::path<3> result = finder.find_path ( origin, target );
    ues::pf::path<3> expected_result;
    // FIXME Temporary implementation until https://svn.boost.org/trac/boost/ticket/9330 is solved.
    expected_result.push_back ( origin );
    expected_result.push_back ( ues::geom::point<3> ( -4, 1, 1 ) );
    expected_result.push_back ( ues::geom::point<3> ( -1, 1, 1 ) );
    expected_result.push_back ( ues::geom::point<3> ( 1, 1, 1 ) );
    expected_result.push_back ( target );

    compare_paths ( expected_result, result );

}


TEST ( pf, visibility_graph_generator_3d_concave )
{
    using ues::pf::graph_pathfinder;

    ues::env::obstacle_vector obstacles;
    obstacles.push_back ( ues::env::obstacle ( { { 0, 0 }, { -3, 0 }, { -3, 1 }, { -1, 1 }, { -1, 2 }, { -3, 2 }, { -3, 3 }, { 0, 3 } }, 8 ) );

    ues::geom::point<3> origin = { -1.5, 1.5, 0 };
    ues::geom::point<3> target = { 0.5, 2, 0 };

    ues::pf::graph_pathfinder<3> finder ( ues::pf::vg3d::visibility_graph_generator::generate_visibility_graph ( obstacles, origin, target ) );

    ues::pf::path<3> result = finder.find_path ( origin, target );
    ues::pf::path<3> expected_result;
    // FIXME Temporary implementation until https://svn.boost.org/trac/boost/ticket/9330 is solved.
    expected_result.push_back ( origin );
    expected_result.push_back ( ues::geom::point<3> ( -3, 2, 0 ) );
    expected_result.push_back ( ues::geom::point<3> ( -3, 3, 0 ) );
    expected_result.push_back ( ues::geom::point<3> ( 0, 3, 0 ) );
    expected_result.push_back ( target );

    compare_paths ( expected_result, result );

}


TEST ( pf, visibility_graph_generator_3d_contained )
{
    using ues::pf::graph_pathfinder;

    ues::env::obstacle_vector obstacles;
    obstacles.push_back ( ues::env::obstacle ( { { 0, 0 }, { 0, 11 }, { 5, 11 }, { 5, 0 } }, 1 ) );
    obstacles.push_back ( ues::env::obstacle ( { { 2, 1 }, { 2, 10 }, { 3, 10 }, { 3, 1 } }, 2 ) );

    ues::geom::point<3> origin = { -1, 2, 0 };
    ues::geom::point<3> target = { 6, 9, 0 };

    ues::pf::graph_pathfinder<3> finder ( ues::pf::vg3d::visibility_graph_generator::generate_visibility_graph ( obstacles, origin, target ) );

    ues::pf::path<3> result = finder.find_path ( origin, target );
    ues::pf::path<3> expected_result;
    // FIXME Temporary implementation until https://svn.boost.org/trac/boost/ticket/9330 is solved.
    expected_result.push_back ( origin );
    expected_result.push_back ( ues::geom::point<3> ( 0, 3, 1 ) );
    expected_result.push_back ( ues::geom::point<3> ( 2, 5, 2 ) );
    expected_result.push_back ( ues::geom::point<3> ( 3, 6, 2 ) );
    expected_result.push_back ( ues::geom::point<3> ( 5, 8, 1 ) );
    expected_result.push_back ( target );

    compare_paths ( expected_result, result );

}
