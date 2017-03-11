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

#include "motion_planner.h"

#include <ompl/util/Console.h>
#include <ompl/base/spaces/SE3StateSpace.h>
#include <ompl/base/objectives/PathLengthOptimizationObjective.h>

#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/bitstar/BITstar.h>
#include <ompl/geometric/PathSimplifier.h>

using namespace ues::pf::mp;


motion_planner::motion_planner ( planner_name used_planner ) noexcept
: used_planner ( std::move ( used_planner ) )
{
}


ues::pf::path<3> motion_planner::plan_motion ( const ues::env::obstacle_vector & obstacles,
                                               const ues::geom::point<3> & origin,
                                               const ues::geom::point<3> & target ) const
{
    ompl::msg::setLogLevel ( ompl::msg::LOG_NONE );

    ompl::base::StateSpacePtr space = compute_state_space ( obstacles, origin, target );

    ompl::base::SpaceInformationPtr si ( new ompl::base::SpaceInformation ( space ) );

    si->setStateValidityChecker ( ompl::base::StateValidityCheckerPtr ( new point_collision_checker ( si, obstacles ) ) );
    si->setMotionValidator ( ompl::base::MotionValidatorPtr ( new segment_collision_checker ( si, obstacles ) ) );

    ompl::base::ScopedState<ompl::base::RealVectorStateSpace> start ( space );
    state_from_point ( *start, origin );

    ompl::base::ScopedState<ompl::base::RealVectorStateSpace> goal ( space );
    state_from_point ( *goal, target );

    ompl::base::ProblemDefinitionPtr pdef ( new ompl::base::ProblemDefinition ( si ) );
    pdef->setStartAndGoalStates ( start, goal );

    ompl::base::OptimizationObjectivePtr oobj ( new ompl::base::PathLengthOptimizationObjective ( si ) );
    pdef->setOptimizationObjective ( oobj );

    ompl::base::PlannerPtr planner = create_planner ( si, used_planner );
    planner->setProblemDefinition ( pdef );
    planner->setup();

    ompl::base::PlannerStatus solved = planner->solve ( 1.0 );

    if ( solved )
    {
        ompl::geometric::PathGeometric & path = static_cast<ompl::geometric::PathGeometric &> ( *pdef->getSolutionPath() );

        ompl::geometric::PathSimplifierPtr simplifier ( new ompl::geometric::PathSimplifier ( si, pdef->getGoal() ) );
        simplifier->simplifyMax ( path );

        ues::pf::path<3> solution;
        for ( ompl::base::State * state : path.getStates() )
        {
            ues::geom::point<3> point;
            point_from_state ( point, *state );
            solution.push_back ( point );
        }

        return solution;
    }
    else
    {
        throw ues::exc::exception ( "OMPL failed to generate solution", UES_CONTEXT );
    }
}


ompl::base::StateSpacePtr motion_planner::compute_state_space ( const ues::env::obstacle_vector & obstacles,
                                                                const ues::geom::point<3> & origin,
                                                                const ues::geom::point<3> & target )
{
    static constexpr ues::math::numeric_type SIZE_INCREMENT = 0.05;

    // Computes the approximate size of the state space.
    const ues::math::numeric_type min_z = 0;
    ues::math::numeric_type max_z = std::max ( origin.get_z(), target.get_z() );
    ues::math::numeric_type min_x = std::min ( origin.get_x(), target.get_x() );
    ues::math::numeric_type max_x = std::max ( origin.get_x(), target.get_x() );
    ues::math::numeric_type min_y = std::min ( origin.get_y(), target.get_y() );
    ues::math::numeric_type max_y = std::max ( origin.get_y(), target.get_y() );

    for ( const ues::env::obstacle & obs : obstacles )
    {
        max_z = std::max ( max_z, obs.get_height() );
        for ( const ues::geom::point<2> & pt : obs.get_shape() )
        {
            min_x = std::min ( min_x, pt.get_x() );
            max_x = std::max ( max_x, pt.get_x() );
            min_y = std::min ( min_y, pt.get_y() );
            max_y = std::max ( max_y, pt.get_y() );
        }
    }

    // Add margins at the sides and on top of the obstacles.
    max_z = max_z * ( SIZE_INCREMENT * 2 + 1 );
    const ues::math::numeric_type x_size = max_x - min_x;
    const ues::math::numeric_type y_size = max_y - min_y;
    min_x -= x_size * SIZE_INCREMENT;
    max_x += x_size * SIZE_INCREMENT;
    min_y -= y_size * SIZE_INCREMENT;
    max_y += y_size * SIZE_INCREMENT;

    // Create the state space object.
    ompl::base::StateSpacePtr space ( new ompl::base::RealVectorStateSpace() );
    space->as<ompl::base::RealVectorStateSpace>()->addDimension ( min_x, max_x );
    space->as<ompl::base::RealVectorStateSpace>()->addDimension ( min_y, max_y );
    space->as<ompl::base::RealVectorStateSpace>()->addDimension ( min_z, max_z );

    return space;
}


void motion_planner::state_from_point ( ompl::base::State & state, const ues::geom::point<3> & point )
{
    ompl::base::RealVectorStateSpace::StateType & rvstate = static_cast<ompl::base::RealVectorStateSpace::StateType &> ( state );
    rvstate[0] = point.get_x();
    rvstate[1] = point.get_y();
    rvstate[2] = point.get_z();
}


void motion_planner::point_from_state ( ues::geom::point<3> & point, const ompl::base::State & state )
{
    const ompl::base::RealVectorStateSpace::StateType & rvstate = static_cast<const ompl::base::RealVectorStateSpace::StateType &> ( state );
    point.set_x ( rvstate[0] );
    point.set_y ( rvstate[1] );
    point.set_z ( rvstate[2] );
}


motion_planner::point_collision_checker::point_collision_checker ( const ompl::base::SpaceInformationPtr & si,
                                                                   const ues::env::obstacle_vector & obstacles )
    : StateValidityChecker ( si ),
      obstacles ( obstacles )
{
}


ompl::base::PlannerPtr motion_planner::create_planner ( const ompl::base::SpaceInformationPtr & si,
                                                        motion_planner::planner_name used_planner )
{
    switch ( used_planner )
    {
    case RRT:
        return ompl::base::PlannerPtr ( new ompl::geometric::RRT ( si ) );
    case PRM:
        return ompl::base::PlannerPtr ( new ompl::geometric::PRM ( si ) );
    case BITstar:
        return ompl::base::PlannerPtr ( new ompl::geometric::BITstar ( si ) );
    default:
        throw ues::exc::exception ( "Unknown motion planner name", UES_CONTEXT );
    }
}


bool motion_planner::point_collision_checker::isValid ( const ompl::base::State * state ) const
{
    ues::geom::point<3> pt;
    point_from_state ( pt, *state );
    bool in_obstacle = obstacles.contains_point ( pt );
    return !in_obstacle;
}


motion_planner::segment_collision_checker::segment_collision_checker ( const ompl::base::SpaceInformationPtr & si,
                                                                       const ues::env::obstacle_vector & obstacles )
    : MotionValidator ( si ),
      obstacles ( obstacles )
{
}


bool motion_planner::segment_collision_checker::checkMotion ( const ompl::base::State * s1,
                                                              const ompl::base::State * s2 ) const
{
    ues::geom::point<3> pt1, pt2;
    point_from_state ( pt1, *s1 );
    point_from_state ( pt2, *s2 );

    ues::geom::segment<3> segment ( pt1, pt2 );

    bool intersects = obstacles.check_intersection ( segment );

    if ( intersects )
    {
        invalid_++;
    }
    else
    {
        valid_++;
    }

    return !intersects;
}


bool motion_planner::segment_collision_checker::checkMotion ( const ompl::base::State * s1,
                                                              const ompl::base::State * s2,
                                                              std::pair< ompl::base::State *, double > & lastValid ) const
{
    ues::geom::point<3> pt1, pt2, ptint;
    point_from_state ( pt1, *s1 );
    point_from_state ( pt2, *s2 );

    ues::geom::segment<3> segment ( pt1, pt2 );

    bool intersects = obstacles.check_intersection ( segment, ptint );

    if ( intersects )
    {
        invalid_++;
        ues::math::numeric_type dist = pt1.distance_to ( pt2 );
        ues::math::numeric_type int_dist = pt1.distance_to ( ptint );
        state_from_point ( *lastValid.first, ptint );
        lastValid.second = int_dist / dist;
    }
    else
    {
        valid_++;
    }

    return !intersects;
}
