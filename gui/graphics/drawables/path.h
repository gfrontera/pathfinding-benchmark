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

#ifndef UES_GUI_GL_PATH_H
#define UES_GUI_GL_PATH_H

#include <QOpenGLFunctions>

#include <pf/path.h>

#include "drawable.h"


namespace ues
{
namespace gui
{
namespace gl
{

class path : public drawable, protected QOpenGLFunctions
{
public:
    path ( const ues::pf::path<3> & path, QVector3D color );

    virtual void paint ( QOpenGLShaderProgram & program );
    
    inline const QVector3D& get_color() const noexcept;

private:
    std::unique_ptr< QVector3D[] > vertices;
    std::size_t num_vertices;
    GLuint buffers[1];
    
    QVector3D color;
};

// Inlined methods.
const QVector3D& path::get_color() const noexcept
{
    return color;
}


}
}
}

#endif // UES_GUI_GL_PATH_H
