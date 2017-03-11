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

#include "text_view.h"
#include "ui_text_view.h"

using namespace ues::gui;


text_view::text_view ( QString tab_name, QString message, QWidget * parent )
    : tab ( std::move ( tab_name ), parent ),
      ui ( new Ui::text_view )
{
    ui->setupUi ( this );
    ui->text_widget->setPlainText ( message );
}


text_view::~text_view()
{
    delete ui;
}
