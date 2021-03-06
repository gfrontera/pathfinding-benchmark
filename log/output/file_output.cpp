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

#include "file_output.h"

#include <log/util.h>

using namespace ues::log;


file_output::file_output ( const std::string & filename )
    : out ( filename )
{
}


void file_output::record ( event e ) noexcept
{
    out << "\t[ " << describe_level ( e.get_level() ) << " ] " << e.get_component() << " -- " << e.get_summary() << std::endl;
    if ( !e.get_message().empty() )
    {
        out << e.get_message() << std::endl;
    }
}
