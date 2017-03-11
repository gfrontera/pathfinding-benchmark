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

#include "envelope.h"

#include <cmath>
#include <limits>

#include <log/logger.h>

using namespace ues::pf::vg2d;

const std::string component_name = "Envelope";


envelope::envelope ( point_rank rank_count ) noexcept
    : sdl ( { { NULL_SEGMENT_INDEX, std::numeric_limits<point_rank>::max() } } ),
      right_zeros ( 1 << ( static_cast< unsigned int > ( std::ceil ( std::log2 ( rank_count ) ) ) + 1 ) ),
      random_access ( std::ceil ( std::log2 ( rank_count ) ) + 1, sdl.end() ),
      present_ranks_vector ( 0 )
{
    random_access.push_back ( sdl.begin() );

    point_rank exponential = 1;
    point_rank power = 0;
    while ( exponential <= right_zeros.size() )
    {
        point_rank next_exponential = exponential << 1;
        for ( point_rank i = exponential; i <= right_zeros.size(); i += next_exponential )
        {
            right_zeros[i - 1] = power;
        }
        ++power;
        exponential = next_exponential;
    }
}


envelope::iterator
envelope::shortest_as_long_as ( const segment_data & s ) noexcept
{
    // Mask the present_ranks_vector so that only segments as long as
    // the one provided are shown.
    point_rank index = ~ ( ( 1 << right_zeros[ s.rank ] ) - 1 );
    index &= present_ranks_vector;
    if ( index )
    {
        unsigned int pos = right_zeros[ index - 1 ];
        assert ( random_access.at ( pos ) != sdl.end() );
        return random_access[pos];
    }
    else
    {
        return random_access.back();
    }
}


envelope::iterator
envelope::pred ( iterator it ) const
{
    return --it;
}


envelope::iterator
envelope::head() noexcept
{
    return sdl.begin();
}


void envelope::insert ( segment_data s, iterator it )
{
    ues::log::logger lg;

    if ( it == sdl.end() || it->segment != NULL_SEGMENT_INDEX )
    {
        ++it;
    }

    unsigned int rz = right_zeros.at ( s.rank );
    assert ( ! ( present_ranks_vector & ( 1 << rz ) ) );
    present_ranks_vector |= 1 << rz;
    sdl.insert ( it, std::move ( s ) );
    random_access[rz] = --it;

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Contents of envelope after insertion" );
        if ( !sdl.empty() )
        {
            std::for_each ( sdl.begin(), sdl.end(), [&e] ( const segment_data & sd ) { if ( sd.segment != NULL_SEGMENT_INDEX ) e.message() << "( " << sd.segment << ", " << sd.rank << " ) "; } );
            e.message() << '\n';
        }
        lg.record ( std::move ( e ) );
    }

}


void envelope::erase ( iterator it )
{
    ues::log::logger lg;

    unsigned int rz = right_zeros.at ( it->rank );
    assert ( present_ranks_vector & ( 1 << rz ) );
    present_ranks_vector &= ~ ( 1 << rz );
    sdl.erase ( it );
    random_access[rz] = sdl.end();

    if ( lg.min_level() <= ues::log::TRACE_LVL )
    {
        ues::log::event e ( ues::log::TRACE_LVL, component_name, "Contents of envelope after deletion" );
        if ( !sdl.empty() )
        {
            std::for_each ( sdl.begin(), sdl.end(), [&e] ( const segment_data & sd ) { if ( sd.segment != NULL_SEGMENT_INDEX ) e.message() << "( " << sd.segment << ", " << sd.rank << " ) "; } );
            e.message() << '\n';
        }
        lg.record ( std::move ( e ) );
    }

}


envelope::iterator envelope::end() noexcept
{
    return sdl.end();
}


bool envelope::empty() const noexcept
{
    return sdl.front().segment == NULL_SEGMENT_INDEX;
}


bool ues::pf::vg2d::operator== ( const envelope::segment_data & s1, const envelope::segment_data & s2 ) noexcept
{
    return s1.rank == s2.rank && s1.segment == s2.segment;
}
