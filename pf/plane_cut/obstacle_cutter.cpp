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

#include "obstacle_cutter.h"

#include <cassert>

#include <log/logger.h>
#include <exc/exception.h>
#include <geom/algorithms_3d.h>

using namespace ues::pf;

const std::string component_name = "Obstacle Cutter";


obstacle_cutter::obstacle_cutter ( ues::math::matrix transformation_matrix )
    : transform ( std::move ( transformation_matrix ) )
{
}


bool obstacle_cutter::cut_obstacle ( const ues::env::obstacle & obs, std::shared_ptr<ues::geom::polygon> & poly ) const
{
    ues::log::logger lg;

    ues::geom::polygon::point_vector points;
    std::vector< ues::geom::point<3> > top, bottom;
    for ( const ues::geom::point<2> & p : obs.get_shape() )
    {
        top.push_back ( ues::geom::point_2d_to_3d ( p, obs.get_height() ) );
        top.back().transform ( transform );

        bottom.push_back ( ues::geom::point_2d_to_3d ( p ) );
        bottom.back().transform ( transform );
    }

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {

        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Obstacle to be cut" );
        e.message() << "Top polygon points: ";
        std::for_each ( top.begin(), top.end(), [&e] ( const ues::geom::point<3> & p ) { e.message() << p << ", "; } );
        e.message() << '\n' << "Bottom polygon points: ";
        std::for_each ( bottom.begin(), bottom.end(), [&e] ( const ues::geom::point<3> & p ) { e.message() << p << ", "; } );
        e.message() << '\n';
        lg.record ( std::move ( e ) );
    }

    enum pos { OVER, MIDDLE, UNDER };
    enum addition_order { TOP_BOTTOM, BOTTOM_TOP, UNDEFINED };

    ues::geom::point<3> * last_point_top, * last_point_bottom;
    last_point_top = &top.back();
    last_point_bottom = &bottom.back();

    addition_order last_order = UNDEFINED;
    pos last_point_pos;
    if ( last_point_top->get_z() > 0 && last_point_bottom->get_z() > 0 )
    {
        last_point_pos = OVER;
    }
    else if ( last_point_top->get_z() <= 0 && last_point_bottom->get_z() <= 0 )
    {
        last_point_pos = UNDER;
    }
    else
    {
        last_point_pos = MIDDLE;
    }

    const unsigned int vertices = top.size();
    for ( unsigned int i = 0; i < vertices; ++i )
    {
        ues::geom::point<3> * current_point_top, * current_point_bottom;
        current_point_top = &top[i];
        current_point_bottom = &bottom[i];

        pos current_point_pos;
        if ( current_point_top->get_z() > 0 && current_point_bottom->get_z() > 0 )
        {
            current_point_pos = OVER;
        }
        else if ( current_point_top->get_z() <= 0 && current_point_bottom->get_z() <= 0 )
        {
            current_point_pos = UNDER;
        }
        else
        {
            current_point_pos = MIDDLE;
        }

        if ( current_point_pos != last_point_pos )
        {
            // The top or the bottom of the obstacle are being intersected.
            std::unique_ptr<ues::geom::point<3>> intersection_top, intersection_bottom;

            if ( ( current_point_top->get_z() <= 0 ) ^ ( last_point_top->get_z() <= 0 ) )
            {
                // The top edge is being intersected.
                ues::geom::segment<3> seg ( *last_point_top, *current_point_top );
                intersection_top.reset ( ues::geom::segment_horizontal_plane_intersection ( seg, 0 ).clone() );
            }
            if ( ( current_point_bottom->get_z() <= 0 ) ^ ( last_point_bottom->get_z() <= 0 ) )
            {
                // The bottom edge is being intersected.
                ues::geom::segment<3> seg ( *last_point_bottom, *current_point_bottom );
                intersection_bottom.reset ( ues::geom::segment_horizontal_plane_intersection ( seg, 0 ).clone() );
            }

            if ( intersection_top.get() == nullptr )
            {
                assert ( intersection_bottom.get() != nullptr );
                points.push_back ( ues::geom::point_3d_to_2d ( *intersection_bottom ) );
            }
            else if ( intersection_bottom.get() == nullptr )
            {
                assert ( intersection_top.get() != nullptr );
                points.push_back ( ues::geom::point_3d_to_2d ( *intersection_top ) );
            }
            else // if ( intersection_top.get() != nullptr && intersection_bottom.get() != nullptr )
            {
                // Determine the order in which points have to be added.
                ues::math::numeric_type dist_top = intersection_top->distance_to ( *last_point_top );
                ues::math::numeric_type dist_bottom = intersection_bottom->distance_to ( *last_point_bottom );
                if ( dist_top < dist_bottom )
                {
                    points.push_back ( ues::geom::point_3d_to_2d ( *intersection_top ) );
                    points.push_back ( ues::geom::point_3d_to_2d ( *intersection_bottom ) );
                    last_order = TOP_BOTTOM;
                }
                else if ( dist_top > dist_bottom )
                {
                    points.push_back ( ues::geom::point_3d_to_2d ( *intersection_bottom ) );
                    points.push_back ( ues::geom::point_3d_to_2d ( *intersection_top ) );
                    last_order = BOTTOM_TOP;
                }
                else // if ( dist_top == dist_bottom )
                {
                    if ( last_order == BOTTOM_TOP )
                    {
                        points.push_back ( ues::geom::point_3d_to_2d ( *intersection_top ) );
                        points.push_back ( ues::geom::point_3d_to_2d ( *intersection_bottom ) );
                        last_order = TOP_BOTTOM;
                    }
                    else
                    {
                        points.push_back ( ues::geom::point_3d_to_2d ( *intersection_bottom ) );
                        points.push_back ( ues::geom::point_3d_to_2d ( *intersection_top ) );
                        last_order = BOTTOM_TOP;
                    }
                }

            }
        }
        if ( current_point_pos == MIDDLE )
        {
            // The vertical edge is being intersected.
            ues::geom::segment<3> seg ( *current_point_bottom, *current_point_top );
            ues::geom::point<3> intersection_point = ues::geom::segment_horizontal_plane_intersection ( seg, 0 );
            points.push_back ( ues::geom::point_3d_to_2d ( intersection_point ) );

        }

        last_point_top = current_point_top;
        last_point_bottom = current_point_bottom;
        last_point_pos = current_point_pos;
    }

    if ( points.size() > 0 )
    {
        poly.reset ( new ues::geom::polygon ( std::move ( points ) ) );
        return true;
    }
    return false;
}


std::vector< ues::geom::polygon > obstacle_cutter::cut_obstacles ( const ues::env::obstacle_vector & obstacles ) const
{
    std::vector< ues::geom::polygon > result;
    for ( const ues::env::obstacle & obs : obstacles )
    {
        std::shared_ptr< ues::geom::polygon > poly;
        if ( cut_obstacle ( obs, poly ) )
        {
            result.push_back ( std::move ( *poly ) );
        }
    }
    return result;
}
