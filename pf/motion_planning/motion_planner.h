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

#ifndef UES_PF_MP_MOTION_PLANNER_H
#define UES_PF_MP_MOTION_PLANNER_H

#include <pf/path.h>
#include <env/obstacle_vector.h>

#include <ompl/base/StateSpace.h>
#include <ompl/base/StateValidityChecker.h>
#include <ompl/base/Planner.h>

namespace ues
{
namespace pf
{
namespace mp
{

class motion_planner
{
public:
    enum planner_name { RRT, PRM, BITstar };
    
    
    motion_planner( planner_name used_planner ) noexcept;

    ues::pf::path< 3 > plan_motion ( const ues::env::obstacle_vector & obstacles,
                                     const ues::geom::point<3> & origin,
                                     const ues::geom::point<3> & target ) const;

private:
    class point_collision_checker : public ompl::base::StateValidityChecker
    {
    public:
        point_collision_checker ( const ompl::base::SpaceInformationPtr & si,
                                  const ues::env::obstacle_vector & obstacles );

        bool isValid ( const ompl::base::State * state ) const override;

    private:
        const ues::env::obstacle_vector & obstacles;

    };

    class segment_collision_checker : public ompl::base::MotionValidator
    {
    public:
        segment_collision_checker( const ompl::base::SpaceInformationPtr& si,
                                   const ues::env::obstacle_vector & obstacles );

        bool checkMotion( const ompl::base::State* s1, const ompl::base::State* s2 ) const;
        bool checkMotion( const ompl::base::State* s1, const ompl::base::State* s2, std::pair< ompl::base::State*, double >& lastValid ) const;

    private:
        const ues::env::obstacle_vector & obstacles;

    };

    static ompl::base::StateSpacePtr compute_state_space ( const ues::env::obstacle_vector & obstacles,
                                                    const ues::geom::point<3> & origin,
                                                    const ues::geom::point<3> & target );

    static void state_from_point ( ompl::base::State & state, const ues::geom::point<3> & point );
    static void point_from_state ( ues::geom::point<3> & point, const ompl::base::State & state );
    
    static ompl::base::PlannerPtr create_planner( const ompl::base::SpaceInformationPtr & si, planner_name used_planner );

    planner_name used_planner;

};

}
}
}

#endif // UES_PF_MP_MOTION_PLANNER_H
