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

#ifndef UES_EXC_EXCEPTION_H
#define UES_EXC_EXCEPTION_H

#include <exception>
#include <string>

#include <boost/current_function.hpp>

namespace ues
{
namespace exc
{

class exception : public std::exception
{
public:
    /** Constructor method using a description of the exception and the context. */
    exception ( std::string description, std::string context ) noexcept;
    /** Constructor method using another exception and the context. This constructor
     * can be used to propagate exception information. */
    exception ( exception && e, std::string context ) noexcept;
    /** Constructor method using another exception, additional description, and the context.
     * This constructor can be used to propagate exception information. */
    exception ( exception && e, std::string description, std::string context ) noexcept;
    
    /** Returns a description of the exception. */
    virtual const char* what() const noexcept override;

private:
    std::string text;
};

// Definition of macros and auxiliary functions.
#ifndef NDEBUG
std::string context_format ( std::string function_name, std::string file_name, unsigned long int line_number ) noexcept;
#define UES_CONTEXT ::ues::exc::context_format( BOOST_CURRENT_FUNCTION, __FILE__, __LINE__ )
#else
#define UES_CONTEXT BOOST_CURRENT_FUNCTION
#endif

}
}

#endif // UES_EXC_EXCEPTION_H
