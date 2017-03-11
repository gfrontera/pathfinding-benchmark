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

#ifndef UES_GUI_PATH_VIEWER_H
#define UES_GUI_PATH_VIEWER_H

#include <sim/pathfinding/simulation.h>

#include <tabs/simulation_result_tab.h>
#include "widgets/simulation_progress.h"

namespace Ui
{
class path_viewer;
}

namespace ues
{
namespace gui
{

class path_viewer : public ues::gui::simulation_result_tab
{
    Q_OBJECT
public:
    explicit path_viewer ( ues::sim::pf::simulation sim, QString tab_name, QWidget * parent = nullptr );
    path_viewer ( const path_viewer & ) = delete;

    ~path_viewer();

    path_viewer & operator= ( const path_viewer & ) = delete;

    void draw_results ( unsigned int );

public slots:
    bool tab_closing() override;
    void start_simulation() override;
    void show_results();

    void on_scenario_combo_box_currentIndexChanged ( int );
    void on_reference_checkbox_stateChanged( int );
    void on_approximate_checkbox_stateChanged( int );
    void on_approximate_filtering_checkbox_stateChanged( int );
    void on_plane_cut_checkbox_stateChanged( int );
    void on_prm_checkbox_stateChanged( int );
    void on_rrt_checkbox_stateChanged( int );
    void on_bitstar_checkbox_stateChanged( int );
    void on_save_image_button_clicked();
    

signals:
    void simulation_complete() override;

private:
    Ui::path_viewer * ui;
    ues::gui::simulation_progress * progress_widget;
    ues::sim::pf::simulation sim;

};

}
}

#endif // UES_GUI_PATH_VIEWER_H
