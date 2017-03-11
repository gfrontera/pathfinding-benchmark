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

#include "path_view.h"

#include <tabs/init.h>

using namespace ues::gui;


path_view::path_view ( QWidget * parent )
    : environment_view ( parent )
{
}


void path_view::init_shaders()
{
    // Override system locale until shaders are compiled.
    setlocale ( LC_NUMERIC, "C" );

    // Compile vertex and fragment shaders
    program.addShaderFromSourceFile ( QOpenGLShader::Vertex, ":/vpathshader.glsl" );
    program.addShaderFromSourceFile ( QOpenGLShader::Fragment, ":/fpathshader.glsl" );

    // Link shader pipeline.
    program.link();

    // Restore system locale.
    setlocale ( LC_ALL, "" );
}


void path_view::initializeGL()
{
    ues::gui::environment_view::initializeGL();

    init_shaders();
}


void path_view::paintGL()
{
    environment_view::paintGL();

    // Bind this program.
    program.bind();

    program.setUniformValue ( "u_mvp_matrix", get_mvp_matrix() );
    glLineWidth(2.0f);

    for ( ues::gui::gl::path & current_path : paths )
    {
        program.setUniformValue ( "u_color", current_path.get_color() );
        current_path.paint ( program );
    }
    glLineWidth(1.0f);
}


void path_view::set_paths ( const std::vector< ues::pf::path< 3 > > & paths, std::vector< QVector3D > colors )
{
    this->paths.clear();
    for ( unsigned int i = 0; i < paths.size(); ++i )
    {
        this->paths.push_back ( ues::gui::gl::path ( paths[i], std::move ( colors.at ( i ) ) ) );
    }
}
