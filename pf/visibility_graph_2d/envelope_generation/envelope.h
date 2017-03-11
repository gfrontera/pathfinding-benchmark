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

#ifndef UES_PF_VG2D_ENVELOPE_H
#define UES_PF_VG2D_ENVELOPE_H

#include <list>

#include <pf/visibility_graph_2d/util/definitions.h>

namespace ues
{
namespace pf
{
namespace vg2d
{

class envelope
{
public:
    /** Constructor method giving the maximum possible rank. */
    envelope ( point_rank rank_count ) noexcept;
    /** Destructor method. */
    virtual ~envelope() noexcept = default;

    /** Contains the index of the segment and the rank of the last of its points. */
    struct segment_data { segment_index segment; point_rank rank; };
    /** Contains segment data for a series of points. */
    typedef std::list< segment_data > segment_data_list;
    /** Iterator to move around a segment data list. */
    typedef segment_data_list::iterator iterator; //< FIXME Use const_iterator when libstdc++ implementation supports it.

    virtual iterator head() noexcept;
    virtual iterator end() noexcept;
    virtual iterator pred ( iterator ) const;

    virtual iterator shortest_as_long_as ( const segment_data & ) noexcept;

    virtual void insert ( segment_data, iterator );
    virtual void erase ( iterator );

    virtual bool empty() const noexcept;

protected:

    segment_data_list sdl;

    std::vector< unsigned int > right_zeros;
    std::vector< segment_data_list::iterator > random_access;
    point_rank present_ranks_vector;
};

bool operator== ( const envelope::segment_data &, const envelope::segment_data & ) noexcept;

}
}
}

#endif // UES_PF_VG2D_ENVELOPE_H
