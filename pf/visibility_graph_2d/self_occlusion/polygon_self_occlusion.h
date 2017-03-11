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

#ifndef UES_PF_VG2D_POLYGON_SELF_OCCLUSION_H
#define UES_PF_VG2D_POLYGON_SELF_OCCLUSION_H

#include <list>

#include <pf/visibility_graph_2d/util/scenario.h>

namespace ues
{
namespace pf
{
namespace vg2d
{

class polygon_self_occlusion
{
public:
    /** Default constructor. Precomputes the self occlusion information so later
     * queries are resolved in a short time. */
    polygon_self_occlusion ( const scenario & );

    bool check_occluding_segment ( point_index origin_index,
                                   point_index target_index,
                                   segment_index & occluding_segment_index ) const;
private:
    /** Given a polygon and one of the points defining its shape, it contains the angles
     * to the next point to the left and to the right. To determine which point is to the
     * left and which to the right, it is assumed the viewer is looking towards the polygon.
     * Thus, the left point will be the next point clockwise, and the right point will be the
     * next point counter-clockwise.
     * It also contains the index to the occluding segment. */
    struct occlusion_angles
    {
        ues::math::numeric_type right, left;
        segment_index occluding_index;
    };

    /** For a given point, this list contains occlusion angles for every polygon it belongs to. */
    typedef std::list< occlusion_angles > point_occlusion_info;

    /** Shared vector of points present in the scenario. */
    shared_point_vector points;
    /** Vector containing occlusion angles for every point. */
    std::vector< point_occlusion_info > occlusion_info;
};

}
}
}

#endif // UES_PF_VG2D_POLYGON_SELF_OCCLUSION_H
