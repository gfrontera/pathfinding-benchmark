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

#include "tab.h"

using namespace ues::gui;


tab::tab ( QString tab_name, QWidget * parent )
    : QWidget ( parent ),
      tab_name ( std::move ( tab_name ) )
{
}


tab::~tab() noexcept
{
}


const QString & tab::get_tab_name() const noexcept
{
    return tab_name;
}


bool tab::tab_closing()
{
    return true;
}


bool tab::is_unique() const noexcept
{
    return false;
}
