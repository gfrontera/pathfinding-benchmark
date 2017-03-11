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

#ifndef UES_GUI_TAB_H
#define UES_GUI_TAB_H

#include <QWidget>

namespace ues
{
namespace gui
{

class simulation_launcher_tab;

class tab : public QWidget
{
    Q_OBJECT

public:
    /** Constructor method. */
    explicit tab ( QString tab_name, QWidget * parent = nullptr );

    /** Destructor method. */
    virtual ~tab() noexcept;

    const QString & get_tab_name() const noexcept;
    virtual bool is_unique() const noexcept;

public slots:
    virtual bool tab_closing();

signals:
    void tab_created ( tab * new_tab );
    void tab_created ( simulation_launcher_tab * new_tab );

private:
    QString tab_name;

};

}
}

#endif // UES_GUI_TAB_H
