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

#include "utils.h"

#include <stdexcept>

#include <exc/exception.h>

using namespace ues::sim::pf;

std::string ues::sim::pf::describe_output_generator ( OUTPUT_GENERATOR og )
{
    switch ( og )
    {
    case NAIVE:
        return "edge sampling path";
    case APPROXIMATE:
        return "approximate path";
    case APPROXIMATE_BASELINE:
        return "approximate path with obstacle filtering";
    case PLANE_CUT:
        return "plane-cut path";
    case RRT:
        return "rapidly-exploring random trees";
    case PRM:
        return "probabilistic roadmap";
    case BITSTAR:
        return "batch informed trees";
    default:
        throw ues::exc::exception ( "Invalid output generator name", UES_CONTEXT );
    }
}
