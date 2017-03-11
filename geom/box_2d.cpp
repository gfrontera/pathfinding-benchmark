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

#include "box_2d.h"

#include <exc/exception.h>

using namespace ues::geom;


box_2d::box_2d() noexcept
    : min_x ( 0 ),
      min_y ( 0 ),
      max_x ( 0 ),
      max_y ( 0 )
{
}


box_2d::box_2d ( const point<2> & input_point ) noexcept
    : min_x ( input_point.get_x() ),
      min_y ( input_point.get_y() ),
      max_x ( input_point.get_x() ),
      max_y ( input_point.get_y() )
{
}


box_2d::box_2d ( ues::math::numeric_type min_x, ues::math::numeric_type min_y, ues::math::numeric_type max_x, ues::math::numeric_type max_y )
    : min_x ( min_x ),
      min_y ( min_y ),
      max_x ( max_x ),
      max_y ( max_y )
{
    if ( min_x > max_x || min_y > max_y )
    {
        throw ues::exc::exception ( "Minimum and maximum coordinates are invalid", UES_CONTEXT );
    }
}


bool box_2d::contains ( const point<2> & input_point ) const noexcept
{
    return input_point.get_x() >= min_x && input_point.get_x() <= max_x &&
           input_point.get_y() >= min_y && input_point.get_y() <= max_y;
}


bool box_2d::intersects ( const box_2d & input_box ) const noexcept
{
    return ( input_box.get_max_x() > get_min_x() && input_box.get_min_x() < get_max_x() ) &&
           ( input_box.get_max_y() > get_min_y() && input_box.get_min_y() < get_max_y() );
}


void box_2d::include_point ( const point<2> & input_point ) noexcept
{
    min_x = std::min ( min_x, input_point.get_x() );
    min_y = std::min ( min_y, input_point.get_y() );
    max_x = std::max ( max_x, input_point.get_x() );
    max_y = std::max ( max_y, input_point.get_y() );
}
