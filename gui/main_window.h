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

#ifndef UES_GUI_MAIN_WINDOW_H
#define UES_GUI_MAIN_WINDOW_H

#include <queue>

#include <QMainWindow>
#include "tabs/tab.h"
#include "tabs/simulation_result_tab.h"
#include "tabs/simulation_launcher_tab.h"

namespace Ui
{
class main_window;
}

namespace ues
{
namespace gui
{

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    /** Constructor method. */
    explicit main_window ( QWidget * parent = nullptr );
    /** Copy constructor method. */
    main_window ( const main_window & ) = delete;

    /** Destructor method. */
    ~main_window();

    /** Copy assignment operator. */
    main_window & operator= ( const main_window & ) = delete;

public slots:
    void close_tab ( int tab_index );
    void add_tab ( tab * new_tab );
    void add_tab ( simulation_launcher_tab * new_tab );
    void queue_simulation ( simulation_result_tab * simulation_tab );
    void start_next_simulation();

private:
    Ui::main_window * ui;
    std::queue< simulation_result_tab * > simulation_queue;

};

}
}

#endif // UES_GUI_MAIN_WINDOW_H
