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

#ifndef UES_GUI_PATH_VIEW_H
#define UES_GUI_PATH_VIEW_H

#include <pf/path.h>

#include "environment_view.h"
#include "drawables/path.h"

namespace ues
{
namespace gui
{

class path_view : public ues::gui::environment_view
{
    Q_OBJECT

public:
    explicit path_view ( QWidget * parent = nullptr );

    void set_paths ( const std::vector< ues::pf::path<3> > & paths, std::vector< QVector3D > colors );

private:
    /** \name OpenGL functions */
    /** \{ */
    virtual void initializeGL() override;
//     virtual void resizeGL ( int width, int height ) override;
    virtual void paintGL() override;
    /** \} */

    void init_shaders();

    QOpenGLShaderProgram program;

    std::vector< ues::gui::gl::path > paths;
};

}
}

#endif // UES_GUI_PATH_VIEW_H
