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

#include "random_input_generator.h"

#include <boost/random/uniform_real_distribution.hpp>

#include <log/logger.h>
#include <env/obstacle.h>
#include <exc/exception.h>
#include <geom/algorithms_2d.h>

using namespace ues::sim::pf;

const std::string component_name = "Random input generator";

typedef boost::random::uniform_real_distribution< ues::math::numeric_type > uniform_distribution;

const unsigned int random_input_generator::MAX_ITERATIONS_FACTOR = 2;


random_input_generator::random_input_generator () noexcept
    : boundaries ( ues::geom::box_2d ( 0, 0, 100, 100 ) ),
      min_height ( 1 ),
      max_height ( 8 ),
      min_side ( 2 ),
      max_side ( 5 ),
      obstacle_number ( 0 ),
      rotate_obstacles ( true )
{
}


void random_input_generator::set_boundaries ( ues::geom::box_2d boundaries ) noexcept
{
    this->boundaries = std::move ( boundaries );
}


void random_input_generator::set_heights ( ues::math::numeric_type min, ues::math::numeric_type max )
{
    if ( min > max )
    {
        throw ues::exc::exception ( "Invalid obstacle heights (minimum is greater than maximum value)", UES_CONTEXT );
    }
    this->min_height = min;
    this->max_height = max;
}


void random_input_generator::set_sides ( ues::math::numeric_type min, ues::math::numeric_type max )
{
    if ( min > max )
    {
        throw ues::exc::exception ( "Invalid obstacle side lengths (minimum is greater than maximum value)", UES_CONTEXT );
    }
    this->min_side = min;
    this->max_side = max;
}


void random_input_generator::set_obstacle_number ( unsigned int obstacle_number ) noexcept
{
    this->obstacle_number = obstacle_number;
}


void random_input_generator::set_rotation ( bool rotation )
{
    rotate_obstacles = rotation;
}


ues::geom::point<3> random_input_generator::generate_point ( const ues::env::obstacle_vector & obstacles )
{
    uniform_distribution distribution_x ( boundaries.get_min_x(), boundaries.get_max_x() );
    uniform_distribution distribution_y ( boundaries.get_min_y(), boundaries.get_max_y() );
    uniform_distribution distribution_z ( 0, max_height );
    ues::geom::point<3> result = ues::geom::point<3> ( distribution_x ( generator ), distribution_y ( generator ), distribution_z ( generator ) );

    while ( obstacles.contains_point ( result ) )
    {
        result = ues::geom::point<3> ( distribution_x ( generator ), distribution_y ( generator ), distribution_z ( generator ) );
    }

    return result;
}


input random_input_generator::generate_next_input()
{
    ues::log::logger lg;

    input result;

    uniform_distribution distribution_x ( boundaries.get_min_x(), boundaries.get_max_x() );
    uniform_distribution distribution_y ( boundaries.get_min_y(), boundaries.get_max_y() );
    uniform_distribution distribution_z ( 0, max_height );

    uniform_distribution distribution_height ( min_height, max_height );
    uniform_distribution distribution_side ( min_side, max_side );
    uniform_distribution distribution_rotation ( 0, 2 * ues::math::pi );

    const unsigned int max_iterations = obstacle_number * MAX_ITERATIONS_FACTOR;
    unsigned int num_iterations = 0;

    while ( result.environment.get_obstacles().size() < obstacle_number && num_iterations < max_iterations )
    {
        ues::math::numeric_type pos_x = distribution_x ( generator );
        ues::math::numeric_type pos_y = distribution_y ( generator );
        ues::math::numeric_type side_x = ( min_side != max_side ) ? distribution_side ( generator ) : max_side;
        ues::math::numeric_type side_y = ( min_side != max_side ) ? distribution_side ( generator ) : max_side;
        ues::math::numeric_type rotation = ( rotate_obstacles ) ? distribution_rotation ( generator ) : 0;

        ues::geom::polygon poly { { 0, 0 }, { 0, side_y }, { side_x, side_y }, { side_x, 0 } };
        poly.transform ( ues::geom::rotation_matrix_2d ( rotation ) );
        poly.transform ( ues::geom::translation_matrix_2d ( pos_x, pos_y ) );
        ues::env::obstacle obs ( poly, ( min_height != max_height ) ? distribution_height ( generator ) : max_height );

        bool intersection_found = false;
        for ( const ues::env::obstacle & other_obs : result.environment.get_obstacles() )
        {
            if ( poly.check_intersection ( other_obs.get_shape() ) )
            {
                intersection_found = true;
                break;
            }
        }

        if ( !intersection_found )
        {
            result.environment.add_obstacle ( std::move ( obs ) );
        }

        num_iterations++;
    }

    if ( result.environment.get_obstacles().size() < obstacle_number )
    {
        if ( lg.min_level() <= ues::log::WARNING_LVL )
        {
            ues::log::event e ( ues::log::WARNING_LVL, component_name, "Cannot fit that many obstacles in scenario" );
            e.message() << "Trying to generate " << obstacle_number << " obstacles but only " << result.environment.get_obstacles().size() << " could be generated.";
            lg.record ( std::move ( e ) );
        }
    }

    result.origin = generate_point ( result.environment.get_obstacles() );
    result.target = generate_point ( result.environment.get_obstacles() );

    return result;
}
