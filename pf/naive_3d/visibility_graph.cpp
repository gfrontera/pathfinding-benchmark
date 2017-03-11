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
#include <geom/algorithms_3d.h>

using namespace ues::pf::naive_3d;

const ues::math::numeric_type visibility_graph::MAXIMUM_POINT_SEPARATION = 1;


visibility_graph::visibility_graph ( ues::env::obstacle_vector obstacles,
                                     ues::geom::point<3> origin,
                                     ues::geom::point<3> target,
                                     unsigned int cache_size )
    : obstacles ( std::move ( obstacles ) ),
      cache ( cache_size )
{
    insert_point ( std::move ( origin ) );
    insert_point ( std::move ( target ) );

    init();
}


visibility_graph::visibility_graph ( ues::env::obstacle_vector obstacles, ues::geom::point<3> origin, ues::geom::point<3> target )
    : obstacles ( std::move ( obstacles ) )
{
    insert_point ( std::move ( origin ) );
    insert_point ( std::move ( target ) );

    init();

    cache = index_cache ( 10 * points.size() );
}


void visibility_graph::init()
{
    for ( const ues::env::obstacle & obs : obstacles )
    {
        const ues::geom::polygon & poly = obs.get_shape();
        for ( ues::geom::polygon::size_type i = 0; i < poly.size(); ++i )
        {
            const ues::geom::point<2> & current_point = poly.get_point_at ( i );
            const ues::geom::point<2> & next_point = poly.get_point_at ( ( i + 1 ) % poly.size() );

            // First, add points of the vertical edge.
            for ( ues::math::numeric_type point_height = 0; point_height < obs.get_height(); point_height += MAXIMUM_POINT_SEPARATION )
            {
                insert_point ( ues::geom::point_2d_to_3d ( poly.get_point_at ( i ), point_height ) );
            }

            // Then, add points of the horizontal edge to the next vertex.
            ues::math::vector to_next = next_point - current_point;
            ues::math::numeric_type point_distance = norm ( to_next, 2 );
            to_next = to_next / point_distance;
            for ( ues::math::numeric_type traversed_distance = 0; traversed_distance < point_distance; traversed_distance += MAXIMUM_POINT_SEPARATION )
            {
                insert_point ( ues::geom::point_2d_to_3d ( current_point + to_next * traversed_distance, obs.get_height() ) );
            }
        }
    }
}


bool visibility_graph::check_visibility ( size_type point1_index, size_type point2_index, ues::math::numeric_type & distance ) const
{
    size_type min_index = std::min ( point1_index, point2_index );
    size_type max_index = std::max ( point1_index, point2_index );
    point_index_pair index_pair = std::make_pair ( min_index, max_index );

    ues::geom::segment<3> seg ( points[point1_index], points[point2_index] );
    distance = seg.length();
    bool result;

    index_cache::iterator cache_it = cache.find ( index_pair );
    if ( cache_it != cache.end() )
    {
        result = cache_it->second;
    }
    else
    {
        result = !obstacles.check_intersection ( seg );
        cache.insert ( index_pair, result );
    }

    return result;
}


visibility_graph::point_index_vector visibility_graph::adjacents ( size_type point_index ) const
{
    point_index_vector result;
    ues::math::numeric_type dummy_dist;

    for ( size_type i = 0; i < size(); ++i )
    {
        if ( i != point_index && check_visibility ( point_index, i, dummy_dist ) )
        {
            result.push_back ( i );
        }
    }

    return result;
}


visibility_graph::size_type visibility_graph::size() const noexcept
{
    return points.size();
}


visibility_graph::size_type visibility_graph::point_to_index ( const ues::geom::point<3> & point ) const
{
    auto it = point_indices.find ( dynamic_cast<const ues::geom::point<3> &> ( point ) );
    if ( it != point_indices.end() )
    {
        return it->second;
    }
    throw ues::exc::exception ( "Point not found in visibility graph", UES_CONTEXT );
}


const ues::geom::point<3> & visibility_graph::index_to_point ( size_type point_index ) const
{
    return points[point_index];
}


bool visibility_graph::insert_point ( ues::geom::point<3> point )
{
    assert ( point_indices.find ( point ) == point_indices.end() );

    point_indices.insert ( std::make_pair ( point, points.size() ) );
    points.push_back ( std::move ( point ) );
    return true;
}


void visibility_graph::describe ( std::ostream & out ) const noexcept
{
    out << "Visibility graph with obstacles:\n";
    out << obstacles << '\n';
}


std::size_t visibility_graph::index_pair_hash::operator() ( const point_index_pair & p ) const noexcept
{
    std::hash<size_type> f;
    return ( f ( p.first ) << ( sizeof ( std::size_t ) << 2 ) ) ^ f ( p.second );
}
