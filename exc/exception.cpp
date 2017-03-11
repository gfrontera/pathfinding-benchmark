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

#include "exception.h"

#include <sstream>

using namespace ues::exc;


exception::exception ( std::string description, std::string context ) noexcept
{
    std::ostringstream out;
    out << description << ".\n\tIn " << context << '.';
    text = out.str();
}


exception::exception ( exception && e, std::string context ) noexcept
{
    std::ostringstream out;
    out << std::move( e.text ) << '\n';
    out << "\tIn " << context << '.';
    text = out.str();
}


exception::exception ( exception && e, std::string description, std::string context ) noexcept
{
    std::ostringstream out;
    out << std::move( e.text ) << '\n';
    out << description << ".\n\tIn " << context << '.';
    text = out.str();
}


const char * exception::what() const noexcept
{
    return text.c_str();
}


#ifndef NDEBUG

std::string ues::exc::context_format ( std::string function_name, std::string file_name, unsigned long int line_number ) noexcept
{
    std::ostringstream out;
    out << function_name << " (" << file_name << ":" << line_number << ")";
    return out.str();
}

#endif
