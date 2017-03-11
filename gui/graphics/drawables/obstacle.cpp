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

#include "obstacle.h"

using namespace ues::gui::gl;


obstacle::obstacle ( const ues::env::obstacle & obs )
{
    initializeOpenGLFunctions();

    const ues::math::numeric_type & height = obs.get_height();
    const ues::geom::polygon & shape = obs.get_shape();
    const std::size_t num_points = shape.size();
    const std::size_t num_vertices = num_points * 4;
    num_indices = num_points * 6;
    num_polygon_vertices = num_points;
    // Allocate memory for the vertices and normals of the sides of the obstacle.
    vertices.reset ( new QVector3D[ num_vertices ] );
    normals.reset ( new QVector3D[ num_vertices ] );
    // Allocate memory for the vertices and normals of the top of the obstacle.
    polygon_vertices.reset ( new QVector3D[ num_points ] );
    polygon_normals.reset ( new QVector3D[ num_points ] );
    // Allocate memory for all indices.
    indices.reset ( new GLuint[ num_indices ] );

    for ( std::size_t i = 0; i < num_points; ++i )
    {
        const ues::geom::point<2> & current_point = shape.get_point_at ( i );
        const ues::geom::point<2> & next_point = shape.get_point_at ( ( i + 1 ) % num_points );

        // Define the vertices of the sides.
        vertices[4 * i] = QVector3D ( current_point.get_x(), current_point.get_y(), 0 );
        vertices[4 * i + 1] = QVector3D ( current_point.get_x(), current_point.get_y(), height );
        vertices[4 * i + 2] = QVector3D ( next_point.get_x(), next_point.get_y(), 0 );
        vertices[4 * i + 3] = QVector3D ( next_point.get_x(), next_point.get_y(), height );

        QVector3D normal = QVector3D ( current_point.get_y() - next_point.get_y(),  next_point.get_x() - current_point.get_x(), 0 );
        normal.normalize();
        normals[4 * i] = normal;
        normals[4 * i + 1] = normal;
        normals[4 * i + 2] = normal;
        normals[4 * i + 3] = normal;

        indices[6 * i] = 4 * i;
        indices[6 * i + 1] = 4 * i + 1;
        indices[6 * i + 2] = 4 * i + 2;
        indices[6 * i + 3] = 4 * i + 2;
        indices[6 * i + 4] = 4 * i + 1;
        indices[6 * i + 5] = 4 * i + 3;

        // Define the vertices of the top.
        polygon_vertices[num_points - i - 1] = QVector3D ( current_point.get_x(), current_point.get_y(), height );
        polygon_normals[num_points - i - 1] = QVector3D ( 0, 0, 1 );

    }

    // Copy data to buffers.
    glGenBuffers ( 5, buffers );

    // Save vertices of the sides.
    glBindBuffer ( GL_ARRAY_BUFFER, buffers[0] );
    glBufferData ( GL_ARRAY_BUFFER, num_vertices * sizeof ( QVector3D ), vertices.get(), GL_STATIC_DRAW );

    glBindBuffer ( GL_ARRAY_BUFFER, buffers[1] );
    glBufferData ( GL_ARRAY_BUFFER, num_vertices * sizeof ( QVector3D ), normals.get(), GL_STATIC_DRAW );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, buffers[2] );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof ( GLuint ), indices.get(), GL_STATIC_DRAW );

    // Save vertices of the top.
    glBindBuffer ( GL_ARRAY_BUFFER, buffers[3] );
    glBufferData ( GL_ARRAY_BUFFER, num_points * sizeof ( QVector3D ), polygon_vertices.get(), GL_STATIC_DRAW );

    glBindBuffer ( GL_ARRAY_BUFFER, buffers[4] );
    glBufferData ( GL_ARRAY_BUFFER, num_points * sizeof ( QVector3D ), polygon_normals.get(), GL_STATIC_DRAW );

    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
}


void obstacle::paint ( QOpenGLShaderProgram & program )
{
    // Draw sides of the obstacle.
    glBindBuffer ( GL_ARRAY_BUFFER, buffers[0] );
    int vertex_location = program.attributeLocation ( "a_position" );
    program.enableAttributeArray ( vertex_location );
    glVertexAttribPointer ( vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof ( QVector3D ), 0 );

    glBindBuffer ( GL_ARRAY_BUFFER, buffers[1] );
    int normal_location = program.attributeLocation ( "a_normal" );
    program.enableAttributeArray ( normal_location );
    glVertexAttribPointer ( normal_location, 3, GL_FLOAT, GL_FALSE, sizeof ( QVector3D ), 0 );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, buffers[2] );
    glDrawElements ( GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0 );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

    // Draw top of the obstacle.
    glBindBuffer ( GL_ARRAY_BUFFER, buffers[3] );
    vertex_location = program.attributeLocation ( "a_position" );
    program.enableAttributeArray ( vertex_location );
    glVertexAttribPointer ( vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof ( QVector3D ), 0 );

    glBindBuffer ( GL_ARRAY_BUFFER, buffers[4] );
    normal_location = program.attributeLocation ( "a_normal" );
    program.enableAttributeArray ( normal_location );
    glVertexAttribPointer ( normal_location, 3, GL_FLOAT, GL_FALSE, sizeof ( QVector3D ), 0 );

    glDrawArrays ( GL_TRIANGLE_FAN, 0, num_polygon_vertices );

    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

}




