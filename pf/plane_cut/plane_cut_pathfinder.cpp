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

#include "plane_cut_pathfinder.h"

#include <exc/exception.h>
#include <log/logger.h>
#include <geom/algorithms_3d.h>

#include <pf/visibility_graph_2d/visibility_graph_pathfinder.h>
#include "obstacle_cutter.h"

using namespace ues::pf;

const std::string component_name = "3D Plane Cut Pathfinder";

const unsigned int plane_cut_pathfinder::CUT_NUMBER = 4;


path<3> plane_cut_pathfinder::find_path ( const ues::env::obstacle_vector & obstacles,
                                          const ues::geom::point<3> & origin,
                                          const ues::geom::point<3> & target ) const
{
    ues::log::logger lg;

    if ( lg.min_level() <= ues::log::DEBUG_LVL )
    {
        ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Computing plane-cut path" );
        e.message() << "From " << origin << " to " << target << " with " << obstacles.size() << " obstacles:\n";
        e.message() << obstacles << '\n';
        lg.record ( std::move ( e ) );
    }

    try
    {

        path<3> result;

        if ( origin != target )
        {
            ues::math::numeric_type min_length = std::numeric_limits<ues::math::numeric_type>::max();
            ues::math::matrix global_transform = transformation_matrix ( origin, target );
            for ( unsigned int i = 0; i < CUT_NUMBER; ++i )
            {
                try
                {
                    ues::math::numeric_type rotation_angle = ues::math::pi * i / CUT_NUMBER;
                    ues::math::matrix transform = ues::geom::rotation_matrix_3d ( rotation_angle, { 1, 0, 0 } ) * global_transform;
                    ues::math::matrix inverse_transform = transform.i();
                    ues::geom::point<2> transformed_origin;
                    ues::geom::point<2> transformed_target;
                    std::vector< ues::geom::polygon > transformed_obstacles;
                    transform_environment ( std::move ( transform ), obstacles, origin, target, transformed_obstacles, transformed_origin, transformed_target );
                    path<2> path_2d = ues::pf::vg2d::visibility_graph_pathfinder::find_path ( transformed_obstacles, transformed_origin, transformed_target );
                    path<3> temp_result = expand_path ( path_2d, inverse_transform );
                    bool is_valid = true;
                    for ( path<3>::size_type i = 1; i < temp_result.size() - 1; ++i )
                    {
                        if ( dynamic_cast<ues::geom::point<3> &> ( temp_result[i] ).get_z() <= ues::math::epsilon )
                        {
                            is_valid = false;
                        }
                    }
                    if ( is_valid && temp_result.length() < min_length )
                    {
                        result = std::move( temp_result );
                        min_length = result.length();
                    }
                }
                catch ( std::exception & e )
                {
                    if ( lg.min_level() <= ues::log::WARNING_LVL )
                    {
                        ues::log::event evt ( ues::log::WARNING_LVL, component_name, "Plane-cut path failed for a particular plane" );
                        evt.message() << e.what();
                        lg.record ( std::move ( evt ) );
                    }
                }
            }

            if ( min_length == std::numeric_limits<ues::math::numeric_type>::max() )
            {
                throw ues::exc::exception ( "No path could be generated for any of the planes", UES_CONTEXT );
            }
        }
        else
        {
            result.push_back ( std::move( origin ) );
            result.push_back ( std::move( target ) );
        }

        if ( lg.min_level() <= ues::log::DEBUG_LVL )
        {
            ues::log::event e ( ues::log::DEBUG_LVL, component_name, "Found plane-cut path" );
            e.message() << result << '\n';
            lg.record ( std::move ( e ) );
        }

        return result;

    }
    catch ( ues::exc::exception & e )
    {
        throw ues::exc::exception ( std::move(e), "Error computing plane-cut path", UES_CONTEXT );
    }
}


void plane_cut_pathfinder::transform_environment ( ues::math::matrix transform,
                                                   const ues::env::obstacle_vector & obstacles,
                                                   const ues::geom::point<3> & origin,
                                                   const ues::geom::point<3> & target,
                                                   std::vector< ues::geom::polygon > & transformed_obstacles,
                                                   ues::geom::point<2> & transformed_origin,
                                                   ues::geom::point<2> & transformed_target )
{
    ues::geom::point<3> origin_copy = origin;
    ues::geom::point<3> target_copy = target;
    origin_copy.transform ( transform );
    target_copy.transform ( transform );
    transformed_origin = { origin_copy.get_x(), origin_copy.get_y() };
    transformed_target = { target_copy.get_x(), target_copy.get_y() };
    obstacle_cutter oc ( transform );
    transformed_obstacles = oc.cut_obstacles ( obstacles );
}


path< 3 > plane_cut_pathfinder::expand_path ( const path< 2 > & path_2d, const ues::math::matrix & inverse_transformation_matrix )
{
    path< 3 > result;
    for ( const ues::geom::point<2> & p_2d : path_2d )
    {
        ues::geom::point<3> p_3d = ues::geom::point_2d_to_3d ( p_2d, 0 );
        p_3d.transform ( inverse_transformation_matrix );
        result.push_back ( std::move( p_3d ) );
    }
    return result;
}


ues::math::matrix plane_cut_pathfinder::transformation_matrix ( const ues::geom::point<3> & origin, const ues::geom::point<3> & target )
{
    const ues::math::matrix transform1 = ues::geom::translation_matrix_3d ( -origin.get_x(), -origin.get_y(), -origin.get_z() );
    ues::geom::point<3> transformed_target = target;
    transformed_target.transform ( transform1 );

    const ues::math::numeric_type z_rotation = std::atan2 ( transformed_target.get_y(), transformed_target.get_x() );
    const ues::math::matrix transform2 = ues::geom::rotation_matrix_3d ( z_rotation, { 0, 0, -1 } );
    transformed_target.transform ( transform2 );
    assert ( std::abs ( transformed_target.get_y() ) < ues::math::epsilon );

    const ues::math::numeric_type y_rotation = std::atan2 ( transformed_target.get_z(), transformed_target.get_x() );
    const ues::math::matrix transform3 = ues::geom::rotation_matrix_3d ( y_rotation, { 0, 1, 0 } );
    transformed_target.transform ( transform3 );
    assert ( std::abs ( transformed_target.get_z() ) < ues::math::epsilon );

    return transform3 * transform2 * transform1;
}


plane_cut_pathfinder * plane_cut_pathfinder::clone() const &
{
    return new plane_cut_pathfinder ( *this );
}


plane_cut_pathfinder * plane_cut_pathfinder::clone() &&
{
    return new plane_cut_pathfinder ( std::move ( *this ) );
}
