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

#ifndef UES_SIM_PF_STATISTIC_OUTPUT_PROCESSOR_H
#define UES_SIM_PF_STATISTIC_OUTPUT_PROCESSOR_H

#include <sim/base_simulation.h>
#include <sim/pathfinding/output.h>
#include <sim/pathfinding/definitions.h>

namespace ues
{
namespace sim
{
namespace pf
{

class statistic_output_processor : public ues::sim::output_processor< output >
{
public:

    struct statistical_data
    {
        unsigned int sample_size;
        ues::math::numeric_type average_path_length;
        ues::math::numeric_type centile_path_length;
        ues::math::numeric_type average_relative_path_length;
        ues::math::numeric_type centile_relative_path_length;
        boost::posix_time::time_duration average_running_time;
        boost::posix_time::time_duration centile_running_time;
    };

    static const float PERCENTILE;

    /** \name Constructor methods */
    /** \{ */

    statistic_output_processor ( std::vector<OUTPUT_GENERATOR> generators );
    statistic_output_processor ( std::vector<OUTPUT_GENERATOR> generators, OUTPUT_GENERATOR reference );

    /** \} */
    
    /** Destructor method. */
    virtual ~statistic_output_processor() noexcept = default;

    /** \name Methods inherited from output_processor */
    /** \{ */

    void process_output ( const output & ) override;

    void postprocess() override;

    /** \} */

    /** \name Getters */
    /** \{ */

    const std::vector<OUTPUT_GENERATOR> & get_generators() const;
    const std::vector<statistical_data> & get_results() const;

    /** \} */

private:
    struct output_data
    {
        std::vector< ues::math::numeric_type > path_lengths;
        std::vector< ues::math::numeric_type > relative_path_lengths;
        std::vector< boost::posix_time::time_duration > running_times;
    };

    std::vector<OUTPUT_GENERATOR> generators;
    std::vector<output_data> generator_results;
    std::vector<statistical_data> averages;

    std::vector<OUTPUT_GENERATOR>::size_type reference_generator;

};

}
}
}

#endif // UES_SIM_PF_STATISTIC_OUTPUT_PROCESSOR_H
