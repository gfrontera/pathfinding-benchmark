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

#ifndef UES_GUI_INIT_H
#define UES_GUI_INIT_H

#include "tab.h"

namespace Ui
{
class init;
}

namespace ues
{
namespace gui
{

class init : public tab
{
    Q_OBJECT

public:
    /** Constructor method. */
    explicit init ( QWidget * parent = nullptr );
    /** Copy constructor method. */
    init ( const init & ) = delete;

    /** Destructor method. */
    ~init();

    /** Copy assignment method. */
    init& operator=(const init&) = delete;

public slots:
    void on_new_simulation_button_clicked();
    void on_settings_button_clicked();

private:
    Ui::init * ui;

};

}
}

#endif // UES_GUI_INIT_H








