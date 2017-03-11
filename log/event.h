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

#ifndef UES_LOG_EVENT_H
#define UES_LOG_EVENT_H

#include <sstream>

#include <boost/date_time.hpp>

#include "definitions.h"

namespace ues
{
namespace log
{

class event
{
public:
    /** Constructor method. */
    event ( level, std::string component, std::string summary, boost::posix_time::ptime timestamp = boost::posix_time::second_clock::universal_time() ) noexcept;

    /** Copy constructor. */
    event ( const event & ) noexcept = delete;
    /** Move constructor. */
    event ( event && ) = default;

    inline std::ostream & message() noexcept;

    /** \name Getter methods */
    ///\{
    inline boost::posix_time::ptime get_timestamp() const noexcept;
    inline level get_level() const noexcept;
    inline const std::string& get_component() const noexcept;
    inline const std::string& get_summary() const noexcept;
    inline std::string get_message() const noexcept;
    ///\}

private:
    boost::posix_time::ptime timestamp;
    level lvl;
    std::string component;
    std::string summary;
    std::ostringstream msg;

};

// Inlined methods.

std::ostream & event::message() noexcept
{
    return msg;
}


boost::posix_time::ptime event::get_timestamp() const noexcept
{
    return timestamp;
}


level event::get_level() const noexcept
{
    return lvl;
}


const std::string& event::get_component() const noexcept
{
    return component;
}


const std::string& event::get_summary() const noexcept
{
    return summary;
}


std::string event::get_message() const noexcept
{
    return msg.str();
}


}
}

#endif // UES_LOG_EVENT_H
