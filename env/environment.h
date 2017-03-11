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

#ifndef UES_ENV_ENVIRONMENT_H
#define UES_ENV_ENVIRONMENT_H

#include "obstacle_vector.h"

namespace ues
{
namespace env
{

class environment
{
public:

    /** Default constructor method. */
    environment() noexcept;

    /** Adds an obstacle to the environment. */
    void add_obstacle ( obstacle ) noexcept;

    /** \name Getter methods */
    /** \{ */

    inline const obstacle_vector & get_obstacles() const noexcept;

    /** \} */

private:
    obstacle_vector obstacles;

};

// Inlined methods

const obstacle_vector & environment::get_obstacles() const noexcept
{
    return obstacles;
}


}
}

#endif // UES_ENV_ENVIRONMENT_H
