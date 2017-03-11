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

#include "log_widget.h"
#include "ui_log_widget.h"

#include <log/manager.h>
#include <log/logger.h>
#include <tabs/text_view.h>

#include "log/log_output.h"

using namespace ues::gui;


log_widget::log_widget ( QWidget * parent )
    : QWidget ( parent ),
      ui ( new Ui::log_widget ),
      model ( new log_model ( "ues.log" ) )
{
    ui->setupUi ( this );
    ui->component_combo->addItem ( "All components" );

    ues::log::manager::set_default_output ( std::make_shared< log_output > ( model ) );
    ui->log_view->setModel ( model );

    connect ( model, SIGNAL ( new_component ( QString ) ), this, SLOT ( add_component ( QString ) ) );
    connect ( model, SIGNAL ( cleared_components() ), this, SLOT ( clear_components() ) );
}


log_widget::~log_widget()
{
    delete ui;
    delete model;
}


void log_widget::on_clear_log_button_clicked()
{
    model->clear();
}


void log_widget::on_log_view_doubleClicked ( const QModelIndex & index )
{
    QString message = model->get_log_message ( index );
    text_view * log_view = new text_view ( "Log entry", std::move ( message ) );
    emit tab_created ( log_view );
}


void log_widget::on_relevance_combo_currentIndexChanged ( int lvl )
{
    model->set_minimum_level ( static_cast<ues::log::level> ( lvl ) );
}


void log_widget::on_component_combo_currentIndexChanged ( QString component_name )
{
    model->set_component ( std::move ( component_name ) );
}


void log_widget::add_component ( QString component_name )
{
    ui->component_combo->addItem ( component_name );
}


void log_widget::clear_components()
{
    ui->component_combo->setCurrentIndex ( 0 );
    for ( int i = 1; i < ui->component_combo->count(); ++i )
    {
        ui->component_combo->removeItem ( i );
    }
}
