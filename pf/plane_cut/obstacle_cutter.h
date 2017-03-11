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

#ifndef UES_PF_OBSTACLE_CUTTER_H
#define UES_PF_OBSTACLE_CUTTER_H

#include <math/algebra.h>
#include <env/obstacle_vector.h>

namespace ues
{
namespace pf
{

class obstacle_cutter
{
public:
    obstacle_cutter ( ues::math::matrix transformation_matrix );

    /** Returns the cut of the obstacle by the horizontal plane (after applying
     * the transformation matrix to its nodes) */
    bool cut_obstacle ( const ues::env::obstacle &, std::shared_ptr<ues::geom::polygon> & ) const;
    
    std::vector<ues::geom::polygon> cut_obstacles ( const ues::env::obstacle_vector & obstacles ) const;
    
    inline const ues::math::matrix& get_transformation_matrix() const noexcept;
private:
    ues::math::matrix transform;

};

// Inlined methods.

const ues::math::matrix & obstacle_cutter::get_transformation_matrix() const noexcept
{
    return transform;
}

}
}

#endif // UES_PF_OBSTACLE_CUTTER_H
