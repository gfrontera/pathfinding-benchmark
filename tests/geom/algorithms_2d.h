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

#include <geom/algorithms_2d.h>


TEST ( geom, point_to_segment_distance )
{
    {
        ues::geom::point<2> p1 ( 2, 0 );
        ues::geom::point<2> s1 ( 0, 1 );
        ues::geom::point<2> s2 ( 0, -1 );
        ues::math::numeric_type angle = ues::math::pi;

        ues::math::numeric_type result = ues::geom::point_to_segment_distance ( p1, s1, s2, angle );

        ASSERT_FLOAT_EQ ( 2, result ) << "Distance from point " << p1 << " to segment " << s1 << "-" << s2 << " using angle " << angle << " is incorrect.";
    }

    {
        ues::geom::point<2> p1 ( 4, 4 );
        ues::geom::point<2> s1 ( 3, 3.8 );
        ues::geom::point<2> s2 ( 3.8, 3.8 );
        ues::math::numeric_type angle = p1.angle_to ( s1 );

        ues::math::numeric_type result = ues::geom::point_to_segment_distance ( p1, s1, s2, angle );

        ASSERT_FLOAT_EQ ( p1.distance_to ( s1 ), result ) << "Distance from point " << p1 << " to segment " << s1 << "-" << s2 << " using angle " << angle << " is incorrect.";
    }
}
