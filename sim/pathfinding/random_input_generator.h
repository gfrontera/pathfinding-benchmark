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

#ifndef UES_SIM_PF_RANDOM_INPUT_GENERATOR_H
#define UES_SIM_PF_RANDOM_INPUT_GENERATOR_H

#include <boost/random/mersenne_twister.hpp>

#include <sim/base_simulation.h>
#include <sim/pathfinding/input.h>

namespace ues
{
namespace sim
{
namespace pf
{

class random_input_generator : public ues::sim::input_generator< input >
{
public:
    /** Default constructor method */
    random_input_generator() noexcept;
    /** Destructor method. */
    virtual ~random_input_generator() noexcept = default;

    input generate_next_input() override;

    /** \name Setter methods */
    /** \{ */
    void set_boundaries ( ues::geom::box_2d boundaries ) noexcept;
    void set_obstacle_number ( unsigned int obstacle_number ) noexcept;
    void set_heights ( ues::math::numeric_type min, ues::math::numeric_type max );
    void set_sides ( ues::math::numeric_type min, ues::math::numeric_type max );
    void set_rotation( bool rotation );
    /** \} */

private:
    static const unsigned int MAX_ITERATIONS_FACTOR;

    ues::geom::box_2d boundaries;
    ues::math::numeric_type min_height, max_height;
    ues::math::numeric_type min_side, max_side;
    unsigned int obstacle_number;
    bool rotate_obstacles;

    boost::random::mt19937 generator;

    /** Generates a point that is not inside any of the obstacles. */
    ues::geom::point<3> generate_point ( const ues::env::obstacle_vector & obstacles );
};

}
}
}

#endif // UES_SIM_PF_RANDOM_INPUT_GENERATOR_H
