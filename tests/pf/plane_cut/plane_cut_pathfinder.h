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

#include <pf/plane_cut/plane_cut_pathfinder.h>

#include <tests/pf/compare_paths.h>


TEST ( pf, plane_cut_pathfinder_simple )
{
    using ues::pf::plane_cut_pathfinder;

    ues::env::obstacle_vector obstacles;
    obstacles.push_back ( ues::env::obstacle ( { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } }, 2 ) );

    ues::geom::point<3> origin = { 1, 0, 1 };
    ues::geom::point<3> target = { -3, 2, 1 };

    ues::pf::plane_cut_pathfinder pathfinder;

    ues::pf::path<3> result = pathfinder.find_path ( obstacles, origin, target );
    ues::pf::path<3> expected_result;
    // FIXME Temporary implementation until https://svn.boost.org/trac/boost/ticket/9330 is solved.
    // TODO Check the correctness of the result.
//     expected_result.push_back ( origin.clone() );
//     expected_result.push_back ( new ues::geom::point<3> ( 0, 0, 0 ) );
//     expected_result.push_back ( new ues::geom::point<3> ( 1, 0, 0 ) );
//     expected_result.push_back ( target.clone() );
// 
//     compare_paths ( expected_result, result );

}


TEST ( pf, plane_cut_pathfinder_oblique_cut )
{
    using ues::pf::plane_cut_pathfinder;

    ues::env::obstacle_vector obstacles;
    obstacles.push_back ( ues::env::obstacle ( { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } }, 1 ) );

    ues::geom::point<3> origin = { 0, 0, 1.5 };
    ues::geom::point<3> target = { 1.5, 1.5, 0 };

    ues::pf::plane_cut_pathfinder pathfinder;

    ues::pf::path<3> result = pathfinder.find_path ( obstacles, origin, target );
    ues::pf::path<3> expected_result;
    // FIXME Temporary implementation until https://svn.boost.org/trac/boost/ticket/9330 is solved.
    // TODO Check the correctness of the result.
//     expected_result.push_back ( origin.clone() );
//     expected_result.push_back ( new ues::geom::point<3> ( 0, 0, 0 ) );
//     expected_result.push_back ( new ues::geom::point<3> ( 1, 0, 0 ) );
//     expected_result.push_back ( target.clone() );
// 
//     compare_paths ( expected_result, result );

}
