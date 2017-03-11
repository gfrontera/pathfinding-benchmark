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

#ifndef UES_GUI_GL_OBSTACLE_H
#define UES_GUI_GL_OBSTACLE_H

#include <QOpenGLFunctions>

#include <env/obstacle.h>

#include "drawable.h"

namespace ues
{
namespace gui
{
namespace gl
{

class obstacle : drawable, protected QOpenGLFunctions
{
public:
    /** \name Constructor methods */
    /** \{ */

    /** Constructor receiving a obstacle. */
    obstacle ( const ues::env::obstacle & );

    /** Move constructor. */
    obstacle ( obstacle && ) = default;

    /** \} */


    void paint ( QOpenGLShaderProgram & program ) override;

private:
    std::unique_ptr< QVector3D[] > vertices;
    std::unique_ptr< QVector3D[] > normals;
    std::unique_ptr< GLuint[] > indices;
    std::size_t num_indices;

    std::unique_ptr< QVector3D[] > polygon_vertices;
    std::unique_ptr< QVector3D[] > polygon_normals;
    std::size_t num_polygon_vertices;

    GLuint buffers[5];

};

}
}
}

#endif // UES_GUI_GL_OBSTACLE_H
