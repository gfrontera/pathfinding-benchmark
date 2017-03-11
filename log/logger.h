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

#ifndef UES_LOG_LOG_H
#define UES_LOG_LOG_H

#include <memory>

#include "output.h"
#include "event.h"

namespace ues
{
namespace log
{

class logger
{
public:
    /** Default constructor. */
    logger() noexcept;

    /** Constructor providing a log output class. */
    logger ( std::shared_ptr< output > ) noexcept;

    /** Records a message into the log. */
    void record ( event ) noexcept;

    /** Returns the minimum importance level recorded by this logger. */
    inline level min_level() const noexcept;

    /** Changes the minimum importance level recorded by this logger. */
    void set_min_level ( level ) noexcept;

private:
    std::shared_ptr< output > out;
    level min_lvl;
};

// Inlined methods.


level logger::min_level() const noexcept
{
    return min_lvl;
}


}
}

#endif // UES_LOG_LOG_H
