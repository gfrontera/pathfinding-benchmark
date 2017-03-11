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

#ifndef UES_GUI_SIMULATION_LAUNCHER_TAB_H
#define UES_GUI_SIMULATION_LAUNCHER_TAB_H

#include <tabs/tab.h>
#include <tabs/simulation_result_tab.h>

namespace ues
{
namespace gui
{

class simulation_launcher_tab : public ues::gui::tab
{
public:
    /** Constructor method. */
    explicit simulation_launcher_tab ( QString tab_name, QWidget * parent = nullptr );

signals:
    virtual void queued_simulation_created ( simulation_result_tab * ) = 0;

};

}
}

#endif // UES_GUI_SIMULATION_LAUNCHER_TAB_H
