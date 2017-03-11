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

#include <pf/motion_planning/prm_pathfinder.h>


TEST ( pf, PRM_pathfinder_many )
{
    const ues::math::numeric_type max_coord = 4;

    ues::env::obstacle_vector obstacles;
    for ( ues::math::numeric_type i = 0; i < max_coord; i += 1 )
    {
        for ( ues::math::numeric_type j = 0; j < max_coord; j += 1 )
        {
            obstacles.push_back ( ues::env::obstacle ( { { i, j }, { i, j + 0.8 }, { i + 0.8, j + 0.8 }, { i + 0.8, j } }, i + 1 ) );
        }
    }

    ues::geom::point<3> origin = { 0, 0, 0 };
    ues::geom::point<3> target = { max_coord, max_coord, max_coord / 2 };

    ues::pf::mp::prm_pathfinder ppf;
    ues::pf::path<3> result = ppf.find_path ( obstacles, origin, target );

    // This test has been added for benchmarking purposes only.
    SUCCEED();
}
