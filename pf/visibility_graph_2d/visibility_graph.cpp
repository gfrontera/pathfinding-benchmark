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

using namespace ues::pf::vg2d;


visibility_graph::visibility_graph ( shared_point_vector pv, shared_segment_vector sv ) noexcept
    : ues::pf::basic_visibility_graph<2> ( pv->size() ),
      osv ( pv->size() ),
      pv ( std::move ( pv ) ),
      sv ( std::move ( sv ) )
{
    // Fill the point_to_index table
    for ( point_index i = 0; i < this->pv->size(); ++i )
    {
        assert ( pti.find ( (*this->pv)[i] ) == pti.end() );
        pti[ (*this->pv)[i] ] = i;
    }
}


bool visibility_graph::has_point ( const ues::geom::point<2> & point ) const
{
    try
    {
        return pti.find ( static_cast< const ues::geom::point<2> & > ( point ) ) != pti.end();
    }
    catch ( std::bad_cast & e )
    {
        std::ostringstream error;
        error << "Point " << point << " is not a 2D point.";
        throw ues::exc::exception ( error.str(), UES_CONTEXT );
    }
}


void visibility_graph::add_occlusion_segment ( const ues::geom::point<2> & origin, const ues::geom::point<2> & target, segment_index segment )
{
    size_type origin_index = point_to_index ( origin );
    size_type target_index = point_to_index ( target );

    add_occlusion_segment ( origin_index, target_index, segment );
}


void visibility_graph::add_occlusion_segment ( point_index origin, point_index target, segment_index segment )
{
    osv.at ( origin ).insert ( std::make_pair ( target, segment ) );
}


bool visibility_graph::check_occlusion_segment ( const ues::geom::point<2> & origin, const ues::geom::point<2> & target, ues::pf::vg2d::segment_index & occluding_segment ) const
{
    size_type origin_index = point_to_index ( origin );
    size_type target_index = point_to_index ( target );

    return check_occlusion_segment ( origin_index, target_index, occluding_segment );
}


bool visibility_graph::check_occlusion_segment ( ues::pf::vg2d::point_index origin, ues::pf::vg2d::point_index target, ues::pf::vg2d::segment_index & occluding_segment ) const
{
    const occluding_segments & os = osv.at ( origin );
    auto occlusion_it = os.find ( target );
    if ( occlusion_it != os.end() )
    {
        occluding_segment = occlusion_it->second;
        return true;
    }
    else
    {
        return false;
    }
}


void visibility_graph::describe ( std::ostream & out ) const noexcept
{
    ues::pf::basic_visibility_graph<2>::describe( out );
    // Print a first row with the names of the points
    out << "Occluding segments:" << '\n' << '\t';
    for ( size_type i = 0; i < size(); ++i )
    {
        out << i << '\t';
    }
    out << '\n';

    for ( size_type i = 0; i < size(); ++i )
    {
        out << i << '\t';
        for ( size_type j = 0; j < size(); ++j )
        {
            ues::pf::vg2d::segment_index occluding_segment;
            if ( check_occlusion_segment ( i, j, occluding_segment ) )
            {
                out << occluding_segment;
            }
            else
            {
                out << '-';
            }
            out << '\t';
        }
        out << '\n';
    }
}


point_index visibility_graph::point_to_index ( const ues::geom::point<2> & point ) const
{
    point_indices::const_iterator pti_it = pti.find ( dynamic_cast< const ues::geom::point<2> & > ( point ) );
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


const ues::geom::point<2> & visibility_graph::index_to_point ( visibility_graph::size_type point_index ) const
{
    return (*this->pv)[point_index];
}

