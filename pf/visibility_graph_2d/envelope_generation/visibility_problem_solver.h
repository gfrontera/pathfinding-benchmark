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

#ifndef UES_PF_VG2D_VISIBILITY_PROBLEM_SOLVER_H
#define UES_PF_VG2D_VISIBILITY_PROBLEM_SOLVER_H

#include <vector>

#include <pf/visibility_graph_2d/util/definitions.h>

namespace ues
{
namespace pf
{
namespace vg2d
{

class visibility_problem_solver
{
public:
    /** Struct containing the sorted ranks of both points of a segment. */
    struct segment_rank { point_rank min; point_rank max; };
    /** Vector containing the segment rank information for every segment. */
    typedef std::vector< segment_rank > segment_rank_vector;

    /** Struct containint information about the visibility of a point from another point.
     * The boolean value \a visible will be true if and only if the point is visible from
     * the other point. Otherwise, \a segment will contain an index to the segment that blocks
     * the view (if exists). */
    struct point_visibility_info { bool visible; segment_index segment; };

    /** For each point_index, contains visibility info from a given point. */
    typedef std::vector< point_visibility_info > visible_point_vector;

    static void solve_visibility ( const point_vector & points,
                                   const segment_vector & segments,
                                   const angle_vector & angles,
                                   const point_index origin,
                                   const point_index_vector & sorted_points,
                                   const rank_vector & ranks,
                                   const segment_rank_vector & segment_ranks,
                                   visible_point_vector & visible_points );

};

}
}
}

#endif // UES_PF_VG2D_VISIBILITY_PROBLEM_SOLVER_H
