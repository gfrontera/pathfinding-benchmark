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

#include "main_window.h"
#include "ui_main_window.h"

#include <log/logger.h>

using namespace ues::gui;

const std::string component_name = "User interface";


main_window::main_window ( QWidget * parent )
    : QMainWindow ( parent ),
      ui ( new Ui::main_window )
{
    ui->setupUi ( this );
    ui->splitter->setCollapsible ( 0, false );

    connect ( ui->init_tab, SIGNAL ( tab_created ( tab * ) ), this, SLOT ( add_tab ( tab * ) ) );
    connect ( ui->init_tab, SIGNAL ( tab_created ( simulation_launcher_tab * ) ), this, SLOT ( add_tab ( simulation_launcher_tab * ) ) );

}


main_window::~main_window()
{
    delete ui;
}


void main_window::close_tab ( int tab_index )
{
    if ( tab_index == 0 )
    {
        close();
    }
    else
    {
        tab * closing_tab = dynamic_cast<tab *> ( ui->tabs->widget ( tab_index ) );
        if ( closing_tab != nullptr && closing_tab->tab_closing() )
        {
            ui->tabs->removeTab ( tab_index );
            if ( !closing_tab->is_unique() )
            {
                closing_tab->deleteLater();
            }
        }
    }
}


void main_window::add_tab ( tab * new_tab )
{
    int tab_index = ui->tabs->indexOf ( new_tab );
    if ( tab_index < 0 )
    {
        connect ( new_tab, SIGNAL ( tab_created ( tab * ) ), this, SLOT ( add_tab ( tab * ) ) );
        tab_index = ui->tabs->addTab ( new_tab, new_tab->get_tab_name() );
    }
    ui->tabs->setCurrentIndex ( tab_index );
}


void main_window::add_tab ( simulation_launcher_tab * new_tab )
{
    connect ( new_tab, SIGNAL ( queued_simulation_created ( simulation_result_tab * ) ), this, SLOT ( queue_simulation ( simulation_result_tab * ) ) );
    add_tab ( static_cast< tab * > ( new_tab ) );
}


void main_window::queue_simulation ( simulation_result_tab * simulation_tab )
{
    ues::log::logger lg;

    if ( lg.min_level() <= ues::log::INFO_LVL )
    {
        ues::log::event e ( ues::log::INFO_LVL, component_name, "Simulation added to the queue" );
        lg.record ( std::move ( e ) );
    }

    if ( simulation_queue.empty() )
    {
        simulation_queue.push ( nullptr );
        simulation_queue.push ( simulation_tab );
        start_next_simulation();
    }
    else
    {
        simulation_queue.push ( simulation_tab );
    }
}


void main_window::start_next_simulation()
{
    simulation_queue.pop();
    if ( !simulation_queue.empty() )
    {
        ues::log::logger lg;

        if ( lg.min_level() <= ues::log::INFO_LVL )
        {
            ues::log::event e ( ues::log::INFO_LVL, component_name, "Queued simulation started" );
            lg.record ( std::move ( e ) );
        }

        simulation_result_tab * simulation_tab = simulation_queue.front();
        connect ( simulation_tab, SIGNAL ( simulation_complete() ), this, SLOT ( start_next_simulation() ) );
        simulation_tab->start_simulation();
        add_tab ( simulation_tab );
    }
}
