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

#ifndef UES_GUI_TEXT_VIEW_H
#define UES_GUI_TEXT_VIEW_H

#include <tabs/tab.h>

namespace Ui
{
class text_view;
}

namespace ues
{
namespace gui
{

class text_view : public ues::gui::tab
{
    Q_OBJECT
public:
    /** Constructor method. */
    text_view ( QString tab_name, QString message, QWidget * parent = nullptr );
    /** Copy constructor method. */
    text_view ( const text_view & ) = delete;

    /** Destructor method. */
    ~text_view();

    /** Copy assignment operator. */
    text_view & operator= ( const text_view & ) = delete;

private:
    Ui::text_view * ui;

};

}
}

#endif // UES_GUI_TEXT_VIEW_H
