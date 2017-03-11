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

#ifndef UES_GUI_SETTINGS_H
#define UES_GUI_SETTINGS_H

#include <memory>

#include <tabs/tab.h>

namespace Ui
{
class settings;
}

namespace ues
{
namespace gui
{

class settings : public ues::gui::tab
{
    Q_OBJECT

public:
    /** Instance access method. */
    static ues::gui::settings * get_instance();
    /** Copy constructor method. */
    settings ( const settings & ) = delete;

    /** Destructor method. */
    ~settings();

    /** Copy assignment operator. */
    settings & operator= ( const settings & ) = delete;

    bool is_unique() const noexcept override;

public slots:
    void on_log_combo_currentIndexChanged ( int );

private:
    /** Private constructor method. */
    explicit settings ( QWidget * parent = nullptr );

    Ui::settings * ui;
    static std::unique_ptr<ues::gui::settings> instance;

};

}
}

#endif // UES_GUI_SETTINGS_H
