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

#include "polygon_self_occlusion.h"

#include <exc/exception.h>
#include <log/logger.h>

using namespace ues::pf::vg2d;

const std::string component_name = "Polygon Self Occlusion";


polygon_self_occlusion::polygon_self_occlusion ( const scenario & current_scenario )
    : points ( current_scenario.get_shared_points() ),
      occlusion_info ( this->points->size() )
{
    ues::log::logger lg;

    const segment_vector & segments = current_scenario.get_segments();

    for ( const polygon & po : current_scenario.get_polygons() )
    {
        for ( segment_index current_segment_index = 0; current_segment_index < po.size(); ++current_segment_index )
        {
            segment_index next_segment_index = ( current_segment_index + 1 ) % po.size();

            point_index central_point_index, right_point_index, left_point_index;

            // Four possible combinations for the points.
            if ( segments[po[current_segment_index]].first == segments[po[next_segment_index]].first )
            {
                central_point_index = segments[po[current_segment_index]].first;
                right_point_index = segments[po[current_segment_index]].second;
                left_point_index = segments[po[next_segment_index]].second;
            }
            else if ( segments[po[current_segment_index]].first == segments[po[next_segment_index]].second )
            {
                central_point_index = segments[po[current_segment_index]].first;
                right_point_index = segments[po[current_segment_index]].second;
                left_point_index = segments[po[next_segment_index]].first;
            }
            else if ( segments[po[current_segment_index]].second == segments[po[next_segment_index]].first )
            {
                central_point_index = segments[po[current_segment_index]].second;
                right_point_index = segments[po[current_segment_index]].first;
                left_point_index = segments[po[next_segment_index]].second;
            }
            else if ( segments[po[current_segment_index]].second == segments[po[next_segment_index]].second )
            {
                central_point_index = segments[po[current_segment_index]].second;
                right_point_index = segments[po[current_segment_index]].first;
                left_point_index = segments[po[next_segment_index]].first;
            }
            else
            {
                throw ues::exc::exception ( "Two consecutive segments in a polygon don't share any common point.", UES_CONTEXT );
            }

            const ues::geom::point<2> & central_point = ( *this->points ) [central_point_index];

            occlusion_angles current_point_angles;
            current_point_angles.right = central_point.angle_to ( ( *this->points ) [right_point_index] );
            current_point_angles.left = central_point.angle_to ( ( *this->points ) [left_point_index] );
            current_point_angles.occluding_index = po[current_segment_index];

            occlusion_info[central_point_index].push_back ( current_point_angles );
        }
    }

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Finished computing polygon self occlusion" );
        e.message() << '\t';
        for ( point_vector::size_type i = 0; i < this->points->size(); ++i )
        {
            e.message() << i << '\t';
        }
        e.message() << '\n';
        for ( point_vector::size_type i = 0; i < this->points->size(); ++i )
        {
            e.message() << i << '\t';
            for ( point_vector::size_type j = 0; j < this->points->size(); ++j )
            {
                segment_index occluding_segment_index;
                if ( check_occluding_segment ( i, j, occluding_segment_index ) )
                {
                    e.message() << occluding_segment_index;
                }
                else
                {
                    e.message() << '-';
                }
                e.message() << '\t';
            }
            e.message() << '\n';
        }
        lg.record ( std::move ( e ) );
    }

}


bool polygon_self_occlusion::check_occluding_segment ( point_index origin_index,
                                                       point_index target_index,
                                                       segment_index & occluding_segment_index ) const
{
    // After having computed the angle intervals that go into the polygon (and therefore are forbidden),
    // the other points are traversed to check whether the path to that points goes through such polygon.
    if ( origin_index != target_index && !occlusion_info[origin_index].empty() )
    {
        ues::math::numeric_type angle = points->at ( origin_index ).angle_to ( points->at ( target_index ) );
        for ( const occlusion_angles & oa : occlusion_info[origin_index] )
        {
            // Two different cases here, depending on whether the right and left point angles go through the X-axis (angle pi) or not.
            if ( oa.right <= oa.left )
            {
                if ( oa.right < angle && angle < oa.left )
                {
                    occluding_segment_index = oa.occluding_index;
                    return true;
                }
            }
            else
            {
                if ( oa.right < angle || angle < oa.left )
                {
                    occluding_segment_index = oa.occluding_index;
                    return true;
                }
            }
        }
    }

    return false;
}
