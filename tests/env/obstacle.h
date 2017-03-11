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

#include <env/obstacle.h>


TEST ( env, obstacle_construction )
{
    ues::env::obstacle obs1 ( { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } }, 20 );
    ASSERT_EQ ( obs1.get_height(), 20 ) << "Obstacle height distorted at constructor.";

    ues::env::obstacle obs2 = obs1;
    ASSERT_EQ ( obs1, obs2 ) << "Obstacle copy constructor not working correctly.";

    ues::env::obstacle obs3 = std::move ( obs2 );
    ASSERT_EQ ( obs1, obs3 ) << "Obstacle move constructor not working correctly.";
}


TEST ( env, obstacle_intersection )
{
    ues::env::obstacle obs ( { { -1, -1 }, { -1, 1 }, { 1, 1 }, { 1, -1 } }, 2 );

    ues::geom::point<3> intersection_point;

    ues::geom::segment<3> segment_from_above_first ( { 0, 0, 3 }, { 0, 0, 1 } );
    ASSERT_TRUE ( obs.check_intersection ( segment_from_above_first, intersection_point ) ) << "Intersection of segment " << segment_from_above_first << " not detected.";

    ues::geom::segment<3> segment_from_above_second ( { -3, 0, 4 }, { 3, 0, 0 } );
    ASSERT_TRUE ( obs.check_intersection ( segment_from_above_second, intersection_point ) ) << "Intersection of segment " << segment_from_above_second << " not detected.";

    ues::geom::segment<3> segment_from_above_third ( { 0, 0, 2 }, { 0, 0, 1 } );
    ASSERT_TRUE ( obs.check_intersection ( segment_from_above_third, intersection_point ) ) << "Intersection of segment " << segment_from_above_third << " not detected.";

    ues::geom::segment<3> segment_from_above_fourth ( { -2, 0, 4 }, { 2, 0, 0 } );
    ASSERT_TRUE ( obs.check_intersection ( segment_from_above_fourth, intersection_point ) ) << "Intersection of segment " << segment_from_above_fourth << " not detected.";

    ues::geom::segment<3> segment_from_side_first ( { -2, 0, 0 }, { 0, 0, 0 } );
    ASSERT_TRUE ( obs.check_intersection ( segment_from_side_first, intersection_point ) ) << "Intersection of segment " << segment_from_side_first << " not detected.";

    ues::geom::segment<3> segment_from_side_second ( { 2, 0, 0 }, { -2, 0, 4 } );
    ASSERT_TRUE ( obs.check_intersection ( segment_from_side_second, intersection_point ) ) << "Intersection of segment " << segment_from_side_second << " not detected.";

    ues::geom::segment<3> segment_no_intersection_first ( { 0, 0, 3 }, { 0, 0, 2 } );
    ASSERT_FALSE ( obs.check_intersection ( segment_no_intersection_first, intersection_point ) ) << "False intersection of segment " << segment_no_intersection_first << " detected at " << intersection_point << ".";

    ues::geom::segment<3> segment_no_intersection_second ( { -4, -1, 1 }, { 0, 3, 2 } );
    ASSERT_FALSE ( obs.check_intersection ( segment_no_intersection_second, intersection_point ) ) << "False intersection of segment " << segment_no_intersection_second << " detected at " << intersection_point << ".";

    ues::geom::segment<3> segment_no_intersection_third ( { -2, -2, 2 }, { 2, 2, 2 } );
    ASSERT_FALSE ( obs.check_intersection ( segment_no_intersection_third, intersection_point ) ) << "False intersection of segment " << segment_no_intersection_third << " detected at " << intersection_point << ".";
}
