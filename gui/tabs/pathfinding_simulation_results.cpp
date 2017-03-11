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

#include "pathfinding_simulation_results.h"
#include "ui_pathfinding_simulation_results.h"

#include <QMessageBox>

#include <sim/pathfinding/statistic_output_processor.h>
#include <sim/pathfinding/utils.h>

using namespace ues::gui;

pathfinding_simulation_results::pathfinding_simulation_results ( ues::sim::pf::simulation simulation, QString tab_name, QWidget * parent )
    : simulation_result_tab ( tab_name, parent ),
      ui ( new Ui::pathfinding_simulation_results ),
      progress_widget ( new ues::gui::simulation_progress ( simulation ) ),
      sim ( std::move ( simulation ) )
{
    ui->setupUi ( this );

    ui->vertical_layout->addWidget ( progress_widget );
    ui->results_widget->setVisible ( false );

    connect ( progress_widget, SIGNAL ( simulation_complete() ), this, SLOT ( show_results() ) );
}


pathfinding_simulation_results::~pathfinding_simulation_results() noexcept
{
    delete ui;
}


void pathfinding_simulation_results::start_simulation()
{
    sim.start_simulation();
}


void pathfinding_simulation_results::show_results()
{
    progress_widget->setVisible ( false );
    ui->results_widget->setVisible ( true );

    update_results();

    emit simulation_complete();
}


void pathfinding_simulation_results::update_results()
{

    const ues::sim::pf::statistic_output_processor & sop = dynamic_cast<const ues::sim::pf::statistic_output_processor &> ( sim.get_output_processor() );
    const std::vector<ues::sim::pf::statistic_output_processor::statistical_data> & results = sop.get_results();

    ui->results_table->setColumnCount ( 7 );
    ui->results_table->setRowCount ( results.size() );

    QStringList methods;

    for ( unsigned int i = 0; i < results.size(); ++i )
    {
        QTableWidgetItem * item;
        methods.append ( QString::fromStdString ( ues::sim::pf::describe_output_generator ( sop.get_generators() [i] ) ) );
        item = new QTableWidgetItem ( QString::number ( results[i].sample_size ) );
        item->setFlags ( item->flags() & ~Qt::ItemIsEditable );
        ui->results_table->setItem ( i, 0, item );
        item = new QTableWidgetItem ( QString::number ( results[i].average_path_length ) );
        item->setFlags ( item->flags() & ~Qt::ItemIsEditable );
        ui->results_table->setItem ( i, 1, item );
        item = new QTableWidgetItem ( QString::number ( results[i].centile_path_length ) );
        item->setFlags ( item->flags() & ~Qt::ItemIsEditable );
        ui->results_table->setItem ( i, 2, item );
        item = new QTableWidgetItem ( QString::number ( results[i].average_relative_path_length ) );
        item->setFlags ( item->flags() & ~Qt::ItemIsEditable );
        ui->results_table->setItem ( i, 3, item );
        item = new QTableWidgetItem ( QString::number ( results[i].centile_relative_path_length ) );
        item->setFlags ( item->flags() & ~Qt::ItemIsEditable );
        ui->results_table->setItem ( i, 4, item );
        item = new QTableWidgetItem ( QString::number ( results[i].average_running_time.total_microseconds() / 1000.0 ) );
        item->setFlags ( item->flags() & ~Qt::ItemIsEditable );
        ui->results_table->setItem ( i, 5, item );
        item = new QTableWidgetItem ( QString::number ( results[i].centile_running_time.total_microseconds() / 1000.0 ) );
        item->setFlags ( item->flags() & ~Qt::ItemIsEditable );
        ui->results_table->setItem ( i, 6, item );

    }

    QString centile = QString::number ( static_cast<unsigned int> ( ues::sim::pf::statistic_output_processor::PERCENTILE * 100 ) ) + "%ile.";
    QStringList horizontal_labels = {"Sample size", "Length (mean)", "Length (" + centile + ")", "Length ratio (mean)", "Length ratio (" + centile + ")", "Running time (mean)", "Running time (" + centile + ")" };
    ui->results_table->setHorizontalHeaderLabels ( horizontal_labels );
    ui->results_table->setVerticalHeaderLabels ( methods );

}


bool pathfinding_simulation_results::tab_closing()
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
