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

#include "pathfinding_simulation.h"
#include "ui_pathfinding_simulation.h"

#include <QMessageBox>

#include <sim/pathfinding/random_input_generator.h>
#include <sim/pathfinding/statistic_output_processor.h>
#include <sim/pathfinding/simulation.h>
#include <sim/pathfinding/simple_output_processor.h>
#include <pf/visibility_graph_3d/visibility_graph_pathfinder.h>
#include <pf/blovl/baseline_pathfinder.h>
#include <pf/plane_cut/plane_cut_pathfinder.h>
#include <pf/naive_3d/visibility_graph_pathfinder.h>
#include <pf/motion_planning/rrt_pathfinder.h>
#include <pf/motion_planning/prm_pathfinder.h>
#include <pf/motion_planning/bitstar_pathfinder.h>

#include "pathfinding_simulation_results.h"
#include "path_viewer.h"

using namespace ues::gui;


pathfinding_simulation::pathfinding_simulation ( QString tab_name, QWidget * parent )
    : simulation_launcher_tab ( tab_name, parent ),
      ui ( new Ui::pathfinding_simulation )
{
    ui->setupUi ( this );

    ui->max_obs_side_input->setMinimum ( ui->min_obs_side_input->value() );
    ui->min_obs_side_input->setMaximum ( ui->max_obs_side_input->value() );

    ui->max_obs_height_input->setMinimum ( ui->min_obs_height_input->value() );
    ui->min_obs_height_input->setMaximum ( ui->max_obs_height_input->value() );
}


pathfinding_simulation::~pathfinding_simulation() noexcept
{
    delete ui;
}


std::unique_ptr<simulation_result_tab> pathfinding_simulation::generate_simulation_info()
{
    // Set up algorithms.
    std::vector< ues::sim::pf::OUTPUT_GENERATOR > generators;
    ues::sim::pf::simulation::pathfinder_vector pfv;

    if ( ui->approximate_filtering_checkbox->isChecked() )
    {
        pfv.push_back ( new ues::pf::blovl::baseline_pathfinder() );
        generators.push_back ( ues::sim::pf::APPROXIMATE_BASELINE );
    }
    if ( ui->plane_cut_checkbox->isChecked() )
    {
        pfv.push_back ( new ues::pf::plane_cut_pathfinder() );
        generators.push_back ( ues::sim::pf::PLANE_CUT );
    }
    if ( ui->reference_checkbox->isChecked() )
    {
        pfv.push_back ( new ues::pf::naive_3d::visibility_graph_pathfinder() );
        generators.push_back ( ues::sim::pf::NAIVE );
    }
    if ( ui->approximate_checkbox->isChecked() )
    {
        pfv.push_back ( new ues::pf::vg3d::visibility_graph_pathfinder() );
        generators.push_back ( ues::sim::pf::APPROXIMATE );
    }
    if ( ui->rrt_checkbox->isChecked() )
    {
        pfv.push_back ( new ues::pf::mp::rrt_pathfinder() );
        generators.push_back ( ues::sim::pf::RRT );
    }
    if ( ui->prm_checkbox->isChecked() )
    {
        pfv.push_back ( new ues::pf::mp::prm_pathfinder() );
        generators.push_back ( ues::sim::pf::PRM );
    }
    if ( ui->bitstar_checkbox->isChecked() )
    {
        pfv.push_back ( new ues::pf::mp::bitstar_pathfinder() );
        generators.push_back ( ues::sim::pf::BITSTAR );
    }

    if ( pfv.size() == 0 )
    {
        QMessageBox msgBox ( this );
        msgBox.setText ( QStringLiteral ( "Please, enable at least one pathfinding algorithm." ) );
        msgBox.exec();
        return std::unique_ptr<simulation_result_tab>();
    }

    // Set up input generator.
    std::unique_ptr< ues::sim::input_generator<ues::sim::pf::input> > input_generator;

    if ( ui->random_generated_environment_radio->isChecked() )
    {
        // Use a random input generator.
        std::unique_ptr< ues::sim::pf::random_input_generator > rig ( new ues::sim::pf::random_input_generator () );
        rig->set_obstacle_number ( ui->obstacle_number_input->value() );
        rig->set_boundaries ( ues::geom::box_2d ( 0, 0, ui->scenario_size_x_input->value(), ui->scenario_size_y_input->value() ) );
        rig->set_sides ( ui->min_obs_side_input->value(), ui->max_obs_side_input->value() );
        rig->set_heights ( ui->min_obs_height_input->value(), ui->max_obs_height_input->value() );
        rig->set_rotation ( ui->rotation_checkbox->isChecked() );
        input_generator = std::move ( rig );
    }
    else
    {
        throw ues::exc::exception ( "Unknown scenario generation method", UES_CONTEXT );
    }

    // Set up number of simulations.
    unsigned int num_simulations;

    if ( ui->monte_carlo_radio_button->isChecked() )
    {
        num_simulations = ui->simulation_number_input->value();
    }
    else if ( ui->single_scenario_radio_button->isChecked() )
    {
        num_simulations = 1;
    }
    else
    {
        throw ues::exc::exception ( "Cannot determine the number of simulations to be run", UES_CONTEXT );
    }

    // Set up output processor.
    std::unique_ptr< ues::sim::output_processor<ues::sim::pf::output> > fop;
    if ( ui->statistics_radio_button->isChecked() )
    {
        if ( ui->reference_checkbox->isChecked() )
        {
            fop.reset ( new ues::sim::pf::statistic_output_processor ( generators, ues::sim::pf::NAIVE ) );
        }
        else
        {
            fop.reset ( new ues::sim::pf::statistic_output_processor ( generators ) );
        }
    }
    else if ( ui->full_results_radio_button->isChecked() )
    {
        fop.reset ( new ues::sim::pf::simple_output_processor ( generators ) );
    }
    else
    {
        throw ues::exc::exception ( "Cannot determine the way of showing results", UES_CONTEXT );
    }

    // Set up simulation.
    ues::sim::pf::simulation sim ( std::move ( input_generator ), std::move ( fop ), num_simulations, std::move ( pfv ) );

    if ( ui->statistics_radio_button->isChecked() )
    {
        return std::unique_ptr<simulation_result_tab> ( new pathfinding_simulation_results ( std::move ( sim ), "Simulation results" ) );
    }
    else if ( ui->full_results_radio_button->isChecked() )
    {
        return std::unique_ptr<simulation_result_tab> ( new path_viewer ( std::move ( sim ), "Simulation results" ) );
    }

    throw ues::exc::exception ( "Failed to set up simulation", UES_CONTEXT );
}


void pathfinding_simulation::on_start_simulation_button_clicked()
{
    std::unique_ptr<simulation_result_tab> srt = generate_simulation_info();
    if ( srt.get() != nullptr )
    {
        srt->start_simulation();
        emit tab_created ( srt.release() );
    }
}


void pathfinding_simulation::on_queue_simulation_button_clicked()
{
    std::unique_ptr<simulation_result_tab> srt = generate_simulation_info();
    if ( srt.get() != nullptr )
    {
        emit queued_simulation_created ( srt.release() );
    }
}


void pathfinding_simulation::on_min_obs_side_input_valueChanged ( double new_value )
{
    ui->max_obs_side_input->setMinimum ( new_value );
}


void pathfinding_simulation::on_max_obs_side_input_valueChanged ( double new_value )
{
    ui->min_obs_side_input->setMaximum ( new_value );
}


void pathfinding_simulation::on_min_obs_height_input_valueChanged ( double new_value )
{
    ui->max_obs_height_input->setMinimum ( new_value );
}


void pathfinding_simulation::on_max_obs_height_input_valueChanged ( double new_value )
{
    ui->min_obs_height_input->setMaximum ( new_value );
}


void pathfinding_simulation::on_monte_carlo_radio_button_toggled ( bool checked )
{
    ui->simulation_number_label->setEnabled ( checked );
    ui->simulation_number_input->setEnabled ( checked );
}

