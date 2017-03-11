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

#ifndef UES_GUI_PATHFINDING_SIMULATION_RESULTS_H
#define UES_GUI_PATHFINDING_SIMULATION_RESULTS_H

#include <sim/pathfinding/simulation.h>

#include <gui/tabs/simulation_result_tab.h>
#include "widgets/simulation_progress.h"

namespace Ui
{
class pathfinding_simulation_results;
}

namespace ues
{
namespace gui
{

class pathfinding_simulation_results : public ues::gui::simulation_result_tab
{
    Q_OBJECT

public:
    pathfinding_simulation_results ( ues::sim::pf::simulation sim, QString tab_name, QWidget * parent = nullptr );
    pathfinding_simulation_results ( const pathfinding_simulation_results & ) = delete;
    ~pathfinding_simulation_results() noexcept;

    pathfinding_simulation_results & operator= ( const pathfinding_simulation_results & ) = delete;

public slots:
    bool tab_closing() override;
    void start_simulation() override;
    void show_results();

signals:
    void simulation_complete() override;

private:
    Ui::pathfinding_simulation_results * ui;
    ues::gui::simulation_progress * progress_widget;
    ues::sim::pf::simulation sim;

    void update_results();

};

}
}

#endif // UES_GUI_PATHFINDING_SIMULATION_RESULTS_H
