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

#include "path.h"

#include <geom/point.h>

using namespace ues::gui::gl;


path::path ( const ues::pf::path< 3 > & path, QVector3D color )
{
    initializeOpenGLFunctions();

    num_vertices = path.size();
    vertices.reset ( new QVector3D[num_vertices] );

    for ( typename ues::pf::path<3>::size_type i = 0; i < num_vertices; ++i )
    {
        const ues::geom::point<3> & p = dynamic_cast<const ues::geom::point<3> &> ( path[i] );
        vertices[i] = QVector3D ( p.get_x(), p.get_y(), p.get_z() );
    }

    // Copy data to buffers.
    glGenBuffers ( 1, buffers );

    // Save vertices of the sides.
    glBindBuffer ( GL_ARRAY_BUFFER, buffers[0] );
    glBufferData ( GL_ARRAY_BUFFER, num_vertices * sizeof ( QVector3D ), vertices.get(), GL_STATIC_DRAW );

    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

    this->color = std::move ( color );
}


void path::paint ( QOpenGLShaderProgram & program )
{
    // Draw path.
    glBindBuffer ( GL_ARRAY_BUFFER, buffers[0] );
    int vertex_location = program.attributeLocation ( "a_position" );
    program.enableAttributeArray ( vertex_location );
    glVertexAttribPointer ( vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof ( QVector3D ), 0 );

    glDrawArrays ( GL_LINE_STRIP, 0, num_vertices );

    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
}

