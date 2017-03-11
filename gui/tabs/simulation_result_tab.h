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

#ifndef UES_GUI_SIMULATION_RESULT_TAB_H
#define UES_GUI_SIMULATION_RESULT_TAB_H

#include <gui/tabs/tab.h>

namespace ues
{
namespace gui
{

class simulation_result_tab : public ues::gui::tab
{
public:
    /** Constructor method. */
    explicit simulation_result_tab ( QString tab_name, QWidget * parent = nullptr );

public slots:
    virtual void start_simulation() = 0;

signals:
    virtual void simulation_complete() = 0;

};

}
}

#endif // UES_GUI_SIMULATION_RESULT_TAB_H
