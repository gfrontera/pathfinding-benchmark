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

#include "manager.h"

#include "output/file_output.h"

using namespace ues::log;

/* Initialization of static variables.*/
std::shared_ptr< output > manager::out;
level manager::min_lvl = INFO_LVL;

std::shared_ptr< output > manager::default_output() noexcept
{
    if ( out.get() == nullptr )
    {
        out.reset ( new file_output ( "log.txt" ) );
    }
    return out;
}


void manager::set_default_output ( std::shared_ptr< output > out )
{
    manager::out = out;
}


level manager::min_level() noexcept
{
    return manager::min_lvl;
}


void manager::set_min_level ( level lvl ) noexcept
{
    manager::min_lvl = lvl;
}
