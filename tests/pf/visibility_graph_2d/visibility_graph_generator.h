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

#include <pf/visibility_graph_2d/visibility_graph_generator.h>
#include <pf/visibility_graph_2d/util/scenario.h>


TEST ( pf, visibility_graph_generator_simple )
{
    ues::pf::vg2d::scenario current_scenario ( ues::pf::vg2d::point_vector { { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 0 } },
    ues::pf::vg2d::segment_vector { { 0, 3 } },
    ues::pf::vg2d::polygon_vector { { 0 } } );

    ues::pf::vg2d::visibility_graph_generator graph_generator ( current_scenario.get_shared_points() );
    std::shared_ptr< ues::pf::vg2d::visibility_graph > vg = graph_generator.generate_visibility_graph ( current_scenario.get_shared_segments(),
                                                                                                        current_scenario.get_shared_polygons() );

    ues::math::numeric_type dummy_distance;

    const ues::pf::vg2d::point_vector & points = current_scenario.get_points();
    EXPECT_TRUE ( vg->check_visibility ( points[0], points[1], dummy_distance ) );
    EXPECT_TRUE ( vg->check_visibility ( points[0], points[2], dummy_distance ) );
    EXPECT_TRUE ( vg->check_visibility ( points[0], points[3], dummy_distance ) );
    EXPECT_TRUE ( vg->check_visibility ( points[1], points[3], dummy_distance ) );
    EXPECT_TRUE ( vg->check_visibility ( points[2], points[3], dummy_distance ) );
    EXPECT_FALSE ( vg->check_visibility ( points[1], points[2], dummy_distance ) );
}

TEST ( pf, visibility_graph_generator_one_point )
{
    ues::pf::vg2d::scenario current_scenario ( ues::pf::vg2d::point_vector { { 0, 0 } },
    ues::pf::vg2d::segment_vector {},
    ues::pf::vg2d::polygon_vector {} );

    ues::pf::vg2d::visibility_graph_generator graph_generator ( current_scenario.get_shared_points() );
    std::shared_ptr< ues::pf::vg2d::visibility_graph > vg = graph_generator.generate_visibility_graph ( current_scenario.get_shared_segments(),
                                                                                                        current_scenario.get_shared_polygons() );
}

