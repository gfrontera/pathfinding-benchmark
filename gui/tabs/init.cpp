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

#include "init.h"
#include "ui_init.h"

#include "pathfinding_simulation.h"
#include "simulation_launcher_tab.h"
#include "settings.h"

using namespace ues::gui;


init::init ( QWidget * parent )
    : tab ( "Init", parent ),
      ui ( new Ui::init )
{
    ui->setupUi ( this );
}


init::~init()
{
    delete ui;
}


void init::on_new_simulation_button_clicked()
{
    simulation_launcher_tab * ps = new pathfinding_simulation ( "New simulation" );
    emit tab_created ( ps );
}


void init::on_settings_button_clicked()
{
    emit tab_created ( settings::get_instance() );
}
