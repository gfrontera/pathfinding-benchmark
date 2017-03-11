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

#include "visibility_graph.h"

#include <cassert>

#include <exc/exception.h>

using namespace ues::pf::vg3d;


visibility_graph::visibility_graph() noexcept
    : ues::pf::basic_visibility_graph<3>()
{
}


visibility_graph::visibility_graph ( point_vector pv ) noexcept
    : ues::pf::basic_visibility_graph<3> ( pv.size() ),
      pv ( std::move ( pv ) )
{
    // Populate point-to-index table.
    for ( point_index i = 0; i < this->pv.size(); ++i )
    {
        assert ( pti.find ( this->pv[i] ) == pti.end() );
        pti[ this->pv[i] ] = i;
    }
}


bool visibility_graph::has_point ( const ues::geom::point<3> & point ) const
{
    try
    {
        return pti.find ( dynamic_cast< const ues::geom::point<3> & > ( point ) ) != pti.end();
    }
    catch ( std::bad_cast & e )
    {
        std::ostringstream error;
        error << "Point " << point << " is not a 3D point.";
        throw ues::exc::exception ( error.str(), UES_CONTEXT );
    }
}


void visibility_graph::add_point ( ues::geom::point<3> point )
{
    assert ( pti.find ( point ) == pti.end() );

    size_type new_point_index = pv.size();
    pv.push_back ( std::move ( point ) );
    pti[ pv[new_point_index] ] = new_point_index;

    ues::pf::basic_visibility_graph<3>::add_point();
}


visibility_graph::size_type visibility_graph::point_to_index ( const ues::geom::point<3> & point ) const
{
    point_indices::const_iterator pti_it = pti.find ( dynamic_cast< const ues::geom::point<3> & > ( point ) );
    if ( pti_it != pti.end() )
    {
        return pti_it->second;
    }
    else
    {
        std::ostringstream error;
        error << "Point " << point << " not found in visibility graph";
        throw ues::exc::exception ( error.str(), UES_CONTEXT );
    }
}


const ues::geom::point<3> & ues::pf::vg3d::visibility_graph::index_to_point ( size_type point_index ) const
{
    return pv[point_index];
}

