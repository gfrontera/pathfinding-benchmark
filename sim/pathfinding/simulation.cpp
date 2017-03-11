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

#include "simulation.h"

#include <log/logger.h>

using namespace ues::sim::pf;

const std::string component_name = "Pathfinding simulator";


simulation::simulation ( std::unique_ptr< ues::sim::input_generator< input > > generator,
                         std::unique_ptr< ues::sim::output_processor< output > > processor,
                         unsigned int size,
                         boost::ptr_vector< ues::pf::pathfinder > pathfinders )
    : ues::sim::base_simulation< ues::sim::pf::input, ues::sim::pf::output > ( std::move ( generator ), std::move ( processor ), size ),
      pathfinders ( std::move ( pathfinders ) )
{
}


void simulation::run()
{
    ues::log::logger lg;
    unsigned int i = 0;
    while ( i < get_size() )
    {
        try
        {
            input in = get_input_generator().generate_next_input();
            output out = run_simulation ( std::move ( in ) );
            get_output_processor().process_output ( out );
        }
        catch ( std::exception & e )
        {
            if ( lg.min_level() <= ues::log::ERROR_LVL )
            {
                ues::log::event evt ( ues::log::ERROR_LVL, component_name, "A simulation failed to complete." );
                evt.message() << e.what() << '\n';
                lg.record ( std::move ( evt ) );
            }
        }
        ++i;
        set_progress ( i );
    }
    get_output_processor().postprocess();
}


output simulation::run_simulation ( input in )
{
    output out;
    for ( const ues::pf::pathfinder & pf : pathfinders )
    {
        output_element oe;
        boost::posix_time::ptime time_start = boost::posix_time::microsec_clock::universal_time();
        oe.path = pf.find_path ( in.environment.get_obstacles(), in.origin, in.target );
        oe.running_time = boost::posix_time::microsec_clock::universal_time() - time_start;
        out.algorithm_results.push_back ( oe );
    }
    out.environment = std::move ( in.environment );
    return out;
}
