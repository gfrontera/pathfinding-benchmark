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

#include "obstacle_vector.h"

using namespace ues::env;


bool obstacle_vector::contains_point ( const ues::geom::point<3> & p ) const noexcept
{
    for ( const ues::env::obstacle & obs : *this )
    {
        if ( obs.contains_point ( p ) )
        {
            return true;
        }
    }
    return false;
}


bool obstacle_vector::check_intersection ( const ues::geom::segment<3> & input_segment ) const noexcept
{
    ues::geom::point<3> intersection_point;
    return check_intersection ( input_segment, intersection_point );
}


bool obstacle_vector::check_intersection ( const ues::geom::segment<3> & input_segment,
                                           ues::geom::point<3> & intersection_point ) const noexcept
{
    for ( const ues::env::obstacle & obs : *this )
    {
        if ( obs.check_intersection ( input_segment, intersection_point ) )
        {
            return true;
        }
    }
    return false;
}


std::ostream & ues::env::operator<< ( std::ostream & out, const obstacle_vector & obs_vec )
{
    for ( const ues::env::obstacle & obs : obs_vec )
    {
        out << obs << '\n';
    }
    return out;
}
