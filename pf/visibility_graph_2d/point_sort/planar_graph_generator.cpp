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

#include "planar_graph_generator.h"

#include <log/logger.h>
#include <geom/algorithms_2d.h>

using namespace ues::pf::vg2d;

const std::string component_name = "Planar Graph Generator";


/** Returns a vector with the same points as the input rotated around the origin so that no pair of points
 * have the same x coordinate. */
point_vector remove_extrinsic_degeneracy ( point_vector points, ues::log::logger & lg, const ues::math::numeric_type & epsilon ) noexcept
{
    // If there is only one point, there cannot be any degeneracy.
    if ( points.size() <= 1 )
        return points;

    // Make a copy of the point vector (only maintaining pointers) and sort it.
    std::vector< const ues::geom::point<2> * > copy;
    std::for_each ( points.begin(), points.end(), [&copy] ( const ues::geom::point<2> & point ) { copy.push_back ( &point ); } );
    std::sort ( copy.begin(), copy.end(), [] ( const ues::geom::point<2> * first, const ues::geom::point<2> * second ) { return first->get_x() < second->get_x(); } );

    // Detect whether or not degeneracy exists in the given set of points and
    // get the (non-zero) minimum x distance between any 2 points.
    bool degeneracy_detected = false;
    ues::math::numeric_type min_x_difference = copy.back()->get_x() - copy.front()->get_x();

    {
        auto it = copy.begin();
        ues::math::numeric_type previous_x_coord = ( *it++ )->get_x();

        while ( it != copy.end() )
        {
            ues::math::numeric_type current_x_coord = ( *it++ )->get_x();
            ues::math::numeric_type x_difference = current_x_coord - previous_x_coord;
            if ( x_difference > epsilon )
            {
                if ( x_difference < min_x_difference )
                {
                    min_x_difference = x_difference;
                }
            }
            else
            {
                degeneracy_detected = true;
            }
            previous_x_coord = current_x_coord;
        }
    }

    if ( degeneracy_detected )
    {
        // If degeneracy was detected, then the points must be transformed.

        // Get the maximum y distance between any 2 points.
        auto minmax = std::minmax_element ( copy.begin(), copy.end(), [] ( const ues::geom::point<2> * first, const ues::geom::point<2> * second ) { return first->get_y() < second->get_y(); } );
        ues::math::numeric_type max_y_difference = ( *minmax.second )->get_y() - ( *minmax.first )->get_y();

        // Compute the rotation angle
        ues::math::numeric_type angle = std::atan2 ( max_y_difference, min_x_difference );

        // A 3x3 matrix for homogeneous 2D transformation
        ues::math::matrix transformation = ues::geom::rotation_matrix_2d ( angle / 2 );

        // Apply the transformation
        std::for_each ( points.begin(), points.end(), [&transformation] ( ues::geom::point<2> & point ) { point.transform ( transformation ); } );

        if ( lg.min_level() <= ues::log::TRACE_LVL )
        {
            ues::log::event e ( ues::log::TRACE_LVL, component_name, "Extrinsic degeneration removed" );
            e.message() << "Points rotated " << angle << " rad.\n";
            e.message() << "Points: ";
            std::for_each ( points.begin(), points.end(), [&e] ( const ues::geom::point<2> & point ) { e.message() << point << ", "; } );
            e.message() << '\n';
            lg.record ( std::move ( e ) );
        }

    }

    return points;
}


planar_graph planar_graph_generator::generate_planar_graph ( const point_vector & points, const ues::math::numeric_type & epsilon )
{
    ues::log::logger lg;

    point_vector rotated_points = remove_extrinsic_degeneracy ( points, lg, epsilon );
    planar_graph result ( epsilon );
    for ( const ues::geom::point<2> & point : rotated_points )
    {
        result.add_line ( transform ( point ) );

        if ( lg.min_level() <= ues::log::TRACE_LVL )
        {
            ues::log::event e ( ues::log::TRACE_LVL, component_name, "Finished adding line to planar graph" );
            e.message() << "Result after adding line: " << transform ( point ) << '\n';
            e.message() << result << '\n';
            lg.record ( std::move ( e ) );
        }

    }
    return result;
}


line_2d planar_graph_generator::transform ( const ues::geom::point<2> & p ) noexcept
{
    return line_2d ( -p.get_x(), p.get_y() );
}


ues::geom::point<2> planar_graph_generator::transform ( const line_2d & l ) noexcept
{
    return ues::geom::point<2> ( l.get_x1(), l.get_x0() );
}
