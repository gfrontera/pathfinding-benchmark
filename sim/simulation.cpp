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

#include <exc/exception.h>

using namespace ues::sim;


simulation::simulation ( unsigned int size )
    : size ( size ),
      progress ( 0 ),
      on_progress_update ( [] ( unsigned int ) {} )
{
}


simulation::~simulation() noexcept
{
    if ( thr.joinable() )
    {
        thr.join();
    }
}


void simulation::start_simulation()
{
    thr = std::thread ( &simulation::run, this );
}


void simulation::wait_until_finished()
{
    if ( thr.joinable() )
    {
        thr.join();
    }
}


unsigned int simulation::get_size() const noexcept
{
    return size;
}


unsigned int simulation::get_progress() const noexcept
{
    return progress;
}


void simulation::set_progress ( unsigned int new_progress )
{
    if ( new_progress > size )
    {
        throw ues::exc::exception ( "New progress is greater than the simulation size.", UES_CONTEXT );
    }
    progress = new_progress;
    on_progress_update ( progress );
}


void simulation::set_on_progress_update ( std::function< void ( unsigned int ) > fn ) noexcept
{
    on_progress_update = std::move ( fn );
}
