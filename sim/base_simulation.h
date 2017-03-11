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

#ifndef UES_SIM_BASE_SIMULATION_H
#define UES_SIM_BASE_SIMULATION_H

#include <memory>
#include <boost/concept_check.hpp>

#include <exc/exception.h>
#include <sim/simulation.h>

namespace ues
{
namespace sim
{

template< class TI >
class input_generator
{
public:
    virtual ~input_generator() noexcept = default;
    virtual TI generate_next_input() = 0;
};

template< class TO >
class output_processor
{
public:
    virtual ~output_processor() noexcept = default;
    virtual void process_output ( const TO & ) = 0;
    virtual void postprocess() = 0;
};

template< class TI, class TO >
class base_simulation : public simulation
{
public:
    /** \name Constructor and destructor methods */
    /** \{ */

    base_simulation ( std::unique_ptr< input_generator< TI > >,
                      std::unique_ptr< output_processor< TO > >,
                      unsigned int size );

    base_simulation ( base_simulation && ) = default;

    virtual ~base_simulation() noexcept = default;

    /** \} */

    input_generator< TI > & get_input_generator();
    output_processor< TO > & get_output_processor();

protected:
    virtual void run() = 0;

private:
    std::unique_ptr< input_generator< TI > > generator;
    std::unique_ptr< output_processor< TO > > processor;

};

// Templated methods.

template< class TI, class TO >
base_simulation<TI, TO>::base_simulation ( std::unique_ptr< input_generator< TI > > generator,
                                           std::unique_ptr< output_processor< TO > > processor,
                                           unsigned int size )
    : simulation ( size ),
      generator ( std::move ( generator ) ),
      processor ( std::move ( processor ) )
{
}


template< class TI, class TO >
input_generator< TI > & base_simulation<TI, TO>::get_input_generator()
{
    return *generator;
}


template< class TI, class TO >
output_processor< TO > & base_simulation<TI, TO>::get_output_processor()
{
    return *processor;
}


}
}

#endif // UES_SIM_BASE_SIMULATION_H
