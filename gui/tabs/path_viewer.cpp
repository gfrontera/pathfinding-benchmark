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

#include "path_viewer.h"
#include "ui_path_viewer.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

#include <sim/pathfinding/simple_output_processor.h>

using namespace ues::gui;


path_viewer::path_viewer ( ues::sim::pf::simulation simulation, QString tab_name, QWidget * parent )
    : simulation_result_tab ( tab_name, parent ),
      ui ( new Ui::path_viewer ),
      progress_widget ( new simulation_progress ( simulation ) ),
      sim ( std::move ( simulation ) )
{
    ui->setupUi ( this );

    ui->vertical_layout->addWidget ( progress_widget );
    ui->main_splitter->setVisible ( false );

    connect ( progress_widget, SIGNAL ( simulation_complete() ), this, SLOT ( show_results() ) );
}


path_viewer::~path_viewer()
{
    delete ui;
}


void path_viewer::start_simulation()
{
    sim.start_simulation();
}


void path_viewer::show_results()
{
    progress_widget->setVisible ( false );
    ui->main_splitter->setVisible ( true );

    const ues::sim::pf::simple_output_processor & sop = dynamic_cast<const ues::sim::pf::simple_output_processor &> ( sim.get_output_processor() );
    const unsigned int total_results = sop.get_results().size();

    if ( total_results > 0 )
    {
        ui->scenario_combo_box->clear();
        ui->scenario_combo_box->setEnabled ( true );
        for ( unsigned int i = 1; i <= total_results; ++i )
        {
            ui->scenario_combo_box->addItem ( QString ( "Scenario " ) + QString::number ( i ) );
        }

        for ( ues::sim::pf::OUTPUT_GENERATOR og : sop.get_generators() )
        {
            switch ( og )
            {
            case ues::sim::pf::NAIVE:
                ui->reference_checkbox->setEnabled ( true );
                ui->reference_checkbox->setChecked ( true );
                break;
            case ues::sim::pf::APPROXIMATE:
                ui->approximate_checkbox->setEnabled ( true );
                ui->approximate_checkbox->setChecked ( true );
                break;
            case ues::sim::pf::PLANE_CUT:
                ui->plane_cut_checkbox->setEnabled ( true );
                ui->plane_cut_checkbox->setChecked ( true );
                break;
            case ues::sim::pf::APPROXIMATE_BASELINE:
                ui->approximate_filtering_checkbox->setEnabled ( true );
                ui->approximate_filtering_checkbox->setChecked ( true );
                break;
            case ues::sim::pf::RRT:
                ui->rrt_checkbox->setEnabled ( true );
                ui->rrt_checkbox->setChecked ( true );
                break;
            case ues::sim::pf::PRM:
                ui->prm_checkbox->setEnabled ( true );
                ui->prm_checkbox->setChecked ( true );
                break;
            case ues::sim::pf::BITSTAR:
                ui->bitstar_checkbox->setEnabled ( true );
                ui->bitstar_checkbox->setChecked ( true );
                break;
            }
        }

        draw_results ( 0 );

    }

    emit simulation_complete();

}


void path_viewer::on_scenario_combo_box_currentIndexChanged ( int i )
{
    if ( i >= 0 )
    {
        draw_results ( i );
    }
}


void path_viewer::on_reference_checkbox_stateChanged ( int )
{
    if ( ui->scenario_combo_box->currentIndex() >= 0 )
    {
        draw_results ( ui->scenario_combo_box->currentIndex() );
    }
}


void path_viewer::on_approximate_checkbox_stateChanged ( int )
{
    if ( ui->scenario_combo_box->currentIndex() >= 0 )
    {
        draw_results ( ui->scenario_combo_box->currentIndex() );
    }
}


void path_viewer::on_approximate_filtering_checkbox_stateChanged ( int )
{
    if ( ui->scenario_combo_box->currentIndex() >= 0 )
    {
        draw_results ( ui->scenario_combo_box->currentIndex() );
    }
}


void path_viewer::on_plane_cut_checkbox_stateChanged ( int )
{
    if ( ui->scenario_combo_box->currentIndex() >= 0 )
    {
        draw_results ( ui->scenario_combo_box->currentIndex() );
    }
}


void path_viewer::on_prm_checkbox_stateChanged ( int )
{
    if ( ui->scenario_combo_box->currentIndex() >= 0 )
    {
        draw_results ( ui->scenario_combo_box->currentIndex() );
    }
}


void path_viewer::on_rrt_checkbox_stateChanged ( int )
{
    if ( ui->scenario_combo_box->currentIndex() >= 0 )
    {
        draw_results ( ui->scenario_combo_box->currentIndex() );
    }
}


void path_viewer::on_bitstar_checkbox_stateChanged ( int )
{
    if ( ui->scenario_combo_box->currentIndex() >= 0 )
    {
        draw_results ( ui->scenario_combo_box->currentIndex() );
    }
}


void path_viewer::draw_results ( unsigned int i )
{
    const ues::sim::pf::simple_output_processor & sop = dynamic_cast<const ues::sim::pf::simple_output_processor &> ( sim.get_output_processor() );

    std::vector< ues::pf::path<3> > results;
    std::vector< QVector3D > colors;

    const ues::sim::pf::output & out = sop.get_results().at ( i );
    ui->viewer_widget->set_environment ( out.environment );
    for ( unsigned int j = 0; j < out.algorithm_results.size() ; ++j )
    {
        switch ( sop.get_generators() [j] )
        {
        case ues::sim::pf::NAIVE:
            if ( ui->reference_checkbox->isChecked() )
            {
                colors.push_back ( QVector3D ( .75, .5, .25 ) );
                results.push_back ( out.algorithm_results[j].path );
            }
            break;
        case ues::sim::pf::APPROXIMATE:
            if ( ui->approximate_checkbox->isChecked() )
            {
                colors.push_back ( QVector3D ( 1, 0, 0 ) );
                results.push_back ( out.algorithm_results[j].path );
            }
            break;
        case ues::sim::pf::PLANE_CUT:
            if ( ui->plane_cut_checkbox->isChecked() )
            {
                colors.push_back ( QVector3D ( 1, .5, 0 ) );
                results.push_back ( out.algorithm_results[j].path );
            }
            break;
        case ues::sim::pf::APPROXIMATE_BASELINE:
            if ( ui->approximate_filtering_checkbox->isChecked() )
            {
                colors.push_back ( QVector3D ( 0, 0, 1 ) );
                results.push_back ( out.algorithm_results[j].path );
            }
            break;
        case ues::sim::pf::PRM:
            if ( ui->prm_checkbox->isChecked() )
            {
                colors.push_back ( QVector3D ( 0, .5, .5 ) );
                results.push_back ( out.algorithm_results[j].path );
            }
            break;
        case ues::sim::pf::RRT:
            if ( ui->rrt_checkbox->isChecked() )
            {
                colors.push_back ( QVector3D ( .5, 0, .5 ) );
                results.push_back ( out.algorithm_results[j].path );
            }
            break;
        case ues::sim::pf::BITSTAR:
            if ( ui->bitstar_checkbox->isChecked() )
            {
                colors.push_back ( QVector3D ( 0, .5, 0 ) );
                results.push_back ( out.algorithm_results[j].path );
            }
            break;
        default:
            colors.push_back ( QVector3D ( 0, 0, 0 ) );
            results.push_back ( out.algorithm_results[j].path );
        }

    }

    ui->viewer_widget->set_paths ( results, std::move ( colors ) );

    ui->viewer_widget->updateGL();
}


void path_viewer::on_save_image_button_clicked()
{
    QString path = QFileDialog::getSaveFileName ( this, tr ( "Save Image" ), QStandardPaths::writableLocation ( QStandardPaths::PicturesLocation ),
                                                  tr ( "PNG Image (*.png)" ) );
    if ( !path.isEmpty() )
    {
        QSize size ( 2000, 2000 );
        QImage img = ui->viewer_widget->save_to_image ( size );
        img.save ( path, "png" );
    }
}


bool path_viewer::tab_closing()
{
    if ( sim.get_progress() < sim.get_size() )
    {
        QMessageBox msgBox ( this );
        msgBox.setText ( "Cannot cancel an ongoing simulation." );
        msgBox.exec();

        return false;
    }
    return true;
}
