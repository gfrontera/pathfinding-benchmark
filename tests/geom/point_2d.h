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

#include <geom/point.h>


TEST ( geom, point_2d_construction )
{
    {
        ues::geom::point<2> p1;
        ues::geom::point<2> p2 ( 0, 0 );

        ASSERT_EQ ( p1, p2 ) << "Default point<2> constructor creates a point with coordinates other than 0.";
    }

    {
        ues::geom::point<2> p1 ( 3, 7 );

        ASSERT_EQ ( p1.get_x(), 3 ) << "point<2> constructor creates a point with x coordinate different than the one given.";
        ASSERT_EQ ( p1.get_y(), 7 ) << "point<2> constructor creates a point with y coordinate different than the one given.";
        
        ues::geom::point<2> p2 = p1;
        ASSERT_EQ ( p1, p2 ) << "point<2> copy constructor does not create an equal point.";
        
        ues::geom::point<2> p3 = std::move( p2 );
        ASSERT_EQ ( p1, p3 ) << "point<2> move constructor does not create an equal point.";
        
        ues::geom::point<2> p4;
        p4 = p1;
        ASSERT_EQ ( p1, p4 ) << "point<2> assignment operator does not copy the point.";
    }
}
