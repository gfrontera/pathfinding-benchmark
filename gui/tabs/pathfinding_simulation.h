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

#ifndef UES_GUI_PATHFINDING_SIMULATION_H
#define UES_GUI_PATHFINDING_SIMULATION_H

#include <memory>

#include <gui/tabs/simulation_launcher_tab.h>
#include <gui/tabs/simulation_result_tab.h>

namespace Ui
{
class pathfinding_simulation;
}

namespace ues
{
namespace gui
{

class pathfinding_simulation : public simulation_launcher_tab
{
    Q_OBJECT
public:
    /** Constructor method. */
    pathfinding_simulation ( QString tab_name, QWidget * parent = nullptr );
    /** Copy constructor method. */
    pathfinding_simulation ( const pathfinding_simulation & ) = delete;

    /** Destructor method. */
    virtual ~pathfinding_simulation() noexcept;

    /** Copy assignment operator. */
    pathfinding_simulation& operator= ( const pathfinding_simulation & ) = delete;

    std::unique_ptr<simulation_result_tab> generate_simulation_info();

public slots:
    void on_start_simulation_button_clicked();
    void on_queue_simulation_button_clicked();

    void on_min_obs_side_input_valueChanged ( double );
    void on_max_obs_side_input_valueChanged ( double );
    void on_min_obs_height_input_valueChanged ( double );
    void on_max_obs_height_input_valueChanged ( double );

    void on_monte_carlo_radio_button_toggled ( bool );

signals:
    void queued_simulation_created ( simulation_result_tab * ) override;

private:
    Ui::pathfinding_simulation * ui;

};

}
}

#endif // UES_GUI_PATHFINDING_SIMULATION_H
