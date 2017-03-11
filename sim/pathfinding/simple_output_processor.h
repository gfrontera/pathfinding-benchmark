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

#ifndef UES_SIM_PF_SIMPLE_OUTPUT_PROCESSOR_H
#define UES_SIM_PF_SIMPLE_OUTPUT_PROCESSOR_H

#include <sim/base_simulation.h>
#include <sim/pathfinding/output.h>
#include <sim/pathfinding/definitions.h>

namespace ues
{
namespace sim
{
namespace pf
{

class simple_output_processor : public ues::sim::output_processor< output >
{
public:
    /** Constructor method. */
    simple_output_processor ( std::vector<OUTPUT_GENERATOR> generators );
    /** Destructor method. */
    virtual ~simple_output_processor() noexcept = default;

    /** \name Methods inherited from output_processor */
    /** \{ */

    void process_output ( const output & ) override;

    void postprocess() override;

    /** \} */

    inline const std::vector<OUTPUT_GENERATOR> & get_generators() const noexcept;
    inline const std::vector< output > & get_results() const noexcept;

private:
    std::vector< output > results;
    std::vector<OUTPUT_GENERATOR> generators;

};


// Inlined methods.


const std::vector< OUTPUT_GENERATOR > & simple_output_processor::get_generators() const noexcept
{
    return generators;
}


const std::vector< output > & simple_output_processor::get_results() const noexcept
{
    return results;
}


}
}
}

#endif // UES_SIM_PF_SIMPLE_OUTPUT_PROCESSOR_H
