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

#ifndef UES_GUI_LOG_WIDGET_H
#define UES_GUI_LOG_WIDGET_H

#include <QWidget>

#include <tabs/tab.h>
#include "log_model.h"

namespace Ui
{
class log_widget;
}

namespace ues
{
namespace gui
{

class log_widget : public QWidget
{
    Q_OBJECT
public:
    /** Constructor method. */
    explicit log_widget ( QWidget * parent = nullptr );
    /** Copy constructor method. */
    log_widget ( const log_widget & ) = delete;
    
    /* Destructor method. */
    ~log_widget();

    /** Copy assignment operator. */
    log_widget & operator= ( const log_widget & ) = delete;

signals:
    void tab_created ( tab * new_tab );

public slots:
    void on_clear_log_button_clicked();
    void on_log_view_doubleClicked ( const QModelIndex & );
    void on_relevance_combo_currentIndexChanged ( int );
    void on_component_combo_currentIndexChanged ( QString );

    void add_component ( QString );
    void clear_components();

private:
    Ui::log_widget * ui;
    log_model * model;

};

}
}

#endif // UES_GUI_LOG_WIDGET_H


