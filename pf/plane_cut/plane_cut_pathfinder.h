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

#ifndef UES_PF_PLANE_CUT_PATHFINDER_H
#define UES_PF_PLANE_CUT_PATHFINDER_H

#include <pf/pathfinder.h>

namespace ues
{
namespace pf
{

class plane_cut_pathfinder : public ues::pf::pathfinder
{
public:
    static const unsigned int CUT_NUMBER;
    
    path<3> find_path ( const ues::env::obstacle_vector & obstacles,
                        const ues::geom::point<3> & origin,
                        const ues::geom::point<3> & target ) const;

    static ues::math::matrix transformation_matrix ( const geom::point<3> & origin, const geom::point<3> & target );

    static void transform_environment ( ues::math::matrix transformation_matrix,
                                        const ues::env::obstacle_vector & obstacles,
                                        const ues::geom::point<3> & origin,
                                        const ues::geom::point<3> & target,
                                        std::vector<ues::geom::polygon> & transformed_obstacles,
                                        ues::geom::point<2> & transformed_origin,
                                        ues::geom::point<2> & transformed_target );

    static path<3> expand_path ( const path<2> & path_2d, const ues::math::matrix & inverse_transformation_matrix );

    /** \name Clone methods */
    /** \{ */
    plane_cut_pathfinder * clone() const & ;
    plane_cut_pathfinder * clone() &&;
    /** \} */
};

}
}

#endif // UES_PF_PLANE_CUT_PATHFINDER_H
