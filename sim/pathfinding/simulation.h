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

#ifndef UES_SIM_PF_SIMULATION_H
#define UES_SIM_PF_SIMULATION_H

#include <boost/ptr_container/ptr_vector.hpp>

#include <pf/pathfinder.h>

#include <sim/base_simulation.h>
#include <sim/pathfinding/input.h>
#include <sim/pathfinding/output.h>

namespace ues
{
namespace sim
{
namespace pf
{

class simulation : public ues::sim::base_simulation< input, output >
{
public:
    typedef boost::ptr_vector< ues::pf::pathfinder > pathfinder_vector;

    simulation ( std::unique_ptr< input_generator< input > > generator,
                 std::unique_ptr< output_processor< output > > processor,
                 unsigned int size,
                 pathfinder_vector pathfinders );

private:
    pathfinder_vector pathfinders;

    void run() override;
    output run_simulation ( input );
};

}
}
}

#endif // UES_SIM_PF_SIMULATION_H

