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

#ifndef UES_LOG_MANAGER_H
#define UES_LOG_MANAGER_H

#include <memory>

#include <log/output.h>

namespace ues
{
namespace log
{

class manager
{
public:
    /** Returns a shared pointer to the default log output. */
    static std::shared_ptr< output > default_output() noexcept;

    /** Returns the minimum log level to be generated. */
    static level min_level() noexcept;

    /** Changes the default log output. */
    static void set_default_output ( std::shared_ptr< output > );

    /** Changes the minimum log level that is generated. */
    static void set_min_level ( level ) noexcept;

private:
    static std::shared_ptr< output > out;
    static level min_lvl;

};

}
}

#endif // UES_LOG_MANAGER_H
