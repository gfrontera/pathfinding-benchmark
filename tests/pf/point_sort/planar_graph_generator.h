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

#include <pf/visibility_graph_2d/point_sort/planar_graph_generator.h>


TEST ( pf, planar_graph_generator )
{

    std::vector< ues::geom::point<2> > points;
    points.push_back ( ues::geom::point<2> ( 0, 0 ) );
    points.push_back ( ues::geom::point<2> ( -1, 0 ) );
    points.push_back ( ues::geom::point<2> ( 1, 4 ) );
    points.push_back ( ues::geom::point<2> ( .5, 3 ) );

    ues::pf::vg2d::planar_graph g = ues::pf::vg2d::planar_graph_generator::generate_planar_graph ( points );

    // TODO Check the correctness of the result.

}
