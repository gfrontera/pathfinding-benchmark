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

#include <geom/polygon.h>


TEST ( geom, polygon_construction )
{
    using namespace ues::geom;

    polygon ( { { -1, 1 }, { 1, 1 }, { 1, -1 }, { -1, -1 } } );
}


TEST ( geom, polygon_segment_intersection )
{
    using namespace ues::geom;

    polygon pol ( { { -1, 1 }, { 1, 1 }, { 1, -1 }, { -1, -1 } } );

    point<2> result;

    segment<2> seg1 ( { -2, 0 }, { 0, 0 } );
    ASSERT_TRUE ( pol.check_intersection ( seg1, result ) );

    segment<2> seg2 ( { -2, 0 }, { -2, 3 } );
    ASSERT_FALSE ( pol.check_intersection ( seg2, result ) );

    segment<2> seg3 ( { 1, 1 }, { 2, 2 } );
    ASSERT_FALSE ( pol.check_intersection ( seg3, result ) );

    segment<2> seg4 ( { 1, 0 }, { 2, 2 } );
    ASSERT_FALSE ( pol.check_intersection ( seg4, result ) );

    segment<2> seg5 ( { 1, 0 }, { 0, 0 } );
    ASSERT_TRUE ( pol.check_intersection ( seg5, result ) );

    segment<2> seg6 ( { 0, -1 }, { -2, -1 } );
    ASSERT_FALSE ( pol.check_intersection ( seg6, result ) );

    segment<2> seg7 ( { 0, 1 }, { 1, 1 } );
    ASSERT_FALSE ( pol.check_intersection ( seg7, result ) );
}


TEST ( geom, point_in_polygon_detection )
{
    using namespace ues::geom;

    polygon pol ( { { 0, 1 }, { 1, 0 }, { 0, -1 } } );

    point<2> point1 ( { -2, 0 } );
    ASSERT_FALSE ( pol.is_inside ( point1 ) );

    point<2> point2 ( { 0.5, 0 } );
    ASSERT_TRUE ( pol.is_inside ( point2 ) );

    point<2> point3 ( { 0.5, 0.5 } );
    ASSERT_FALSE ( pol.is_inside ( point3 ) );

    point<2> point4 ( { 1, 0 } );
    ASSERT_FALSE ( pol.is_inside ( point4 ) );

    point<2> point5 ( { 0, 0 } );
    ASSERT_FALSE ( pol.is_inside ( point5 ) );
}


TEST ( geom, polygon_polygon_intersection )
{
    using namespace ues::geom;

    polygon poly1 ( { { -1, 1 }, { 1, 1 }, { 1, -1 }, { -1, -1 } } );

    polygon poly2 ( { { 2, 3 }, { 3, 3 }, { 3, 2 }, { 2, 2 } } );
    ASSERT_FALSE ( poly1.check_intersection ( poly2 ) );

    polygon poly3 ( { { 0, 2 }, { 2, 2 }, { 2, 0 }, { 0, 0 } } );
    ASSERT_TRUE ( poly1.check_intersection ( poly3 ) );

    polygon poly4 ( { { -.5, .5 }, { .5, .5 }, { .5, -.5 }, { -.5, -.5 } } );
    ASSERT_TRUE ( poly1.check_intersection ( poly4 ) );

    polygon poly5 ( { { -.5, 2 }, { .5, 2 }, { .5, -2 }, { -.5, -2 } } );
    ASSERT_TRUE ( poly1.check_intersection ( poly5 ) );

    polygon poly6 ( { { -.5, 0 }, { .5, 0 }, { .5, -2 }, { -.5, -2 } } );
    ASSERT_TRUE ( poly1.check_intersection ( poly6 ) );
}
