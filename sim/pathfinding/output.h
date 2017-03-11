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

#ifndef UES_SIM_PF_OUTPUT_H
#define UES_SIM_PF_OUTPUT_H

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <env/environment.h>
#include <pf/path.h>

namespace ues
{
namespace sim
{
namespace pf
{

struct output_element
{
    ues::pf::path<3> path;
    boost::posix_time::time_duration running_time;
};

struct output {
    ues::env::environment environment;
    std::vector< output_element > algorithm_results;
};

}
}
}

#endif // UES_SIM_PF_OUTPUT_H
