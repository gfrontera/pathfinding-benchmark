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

#include "environment_view.h"
#include "ui_environment_view.h"

#include <QOpenGLFramebufferObject>

#include <tabs/init.h>

using namespace ues::gui;


environment_view::environment_view ( QWidget * parent )
    : QGLWidget ( parent ),
      ui ( new Ui::environment_view )
{
    ui->setupUi ( this );
}


void environment_view::init_shaders()
{
    // Override system locale until shaders are compiled.
    setlocale ( LC_NUMERIC, "C" );

    // Compile vertex and fragment shaders
    program.addShaderFromSourceFile ( QOpenGLShader::Vertex, ":/vshader.glsl" );
    program.addShaderFromSourceFile ( QOpenGLShader::Fragment, ":/fshader.glsl" );

    // Link shader pipeline.
    program.link();

    // Restore system locale.
    setlocale ( LC_ALL, "" );
}


ues::gui::environment_view::~environment_view()
{
    delete ui;
}


void environment_view::initializeGL()
{
    initializeOpenGLFunctions();
    qglClearColor ( Qt::white );

    glEnable ( GL_DEPTH_TEST );
    glEnable ( GL_CULL_FACE );

    init_shaders();

    light_vector = QVector3D ( 1, 2, -1.5 );
    light_vector.normalize();

    view_target = QVector2D ( 500, 500 );
    view_angles = QVector2D ( 270, 45 );
    view_distance = 800;
}


void environment_view::resizeGL ( int width, int height )
{
    glViewport ( 0, 0, width, height );
    float ratio = static_cast<float> ( width ) / ( height ? height : 1 );

    projection.setToIdentity();
    projection.perspective ( 45.0f, ratio, 1.0f, 2000.0f );
    update_view();
}


void environment_view::paintGL()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Bind this program.
    program.bind();

    program.setUniformValue ( "u_mvp_matrix", mvp );
    program.setUniformValue ( "u_light_vector", light_vector );

    if ( env.get() != nullptr )
    {
        env->paint ( program );
    }
}


void environment_view::update_view()
{
    QVector3D from_pt ( view_distance * std::cos ( view_angles.x() * ues::math::pi / 180 ) * std::cos ( view_angles.y() * ues::math::pi / 180 ),
                        view_distance * std::sin ( view_angles.x() * ues::math::pi / 180 ) * std::cos ( view_angles.y() * ues::math::pi / 180 ),
                        view_distance * std::sin ( view_angles.y() * ues::math::pi / 180 ) );
    QVector3D to_pt ( view_target.x(), view_target.y(), 0 );
    view.setToIdentity();
    view.lookAt ( from_pt + to_pt, to_pt, QVector3D ( 0, 0, 1 ) );
    mvp = projection * view;
}


void environment_view::mouseMoveEvent ( QMouseEvent * event )
{
    QWidget::mouseMoveEvent ( event );

    QVector2D movement ( event->x() - reference_position.x(), event->y() - reference_position.y() );

    if ( event->buttons() & Qt::LeftButton )
    {
        view_target += QVector2D ( movement.x() * std::sin ( view_angles.x() * ues::math::pi / 180 ) - movement.y() * std::cos ( view_angles.x() * ues::math::pi / 180 ),
                                   -movement.x() * std::cos ( view_angles.x() * ues::math::pi / 180 ) - movement.y() * std::sin ( view_angles.x() * ues::math::pi / 180 ) )
                       * view_distance / 500;
    }
    else if ( event->buttons() & Qt::MidButton )
    {
        view_angles += ( movement * .5 );
        view_angles.setY ( qMax ( view_angles.y(), 0.0f ) );
        view_angles.setY ( qMin ( view_angles.y(), 89.0f ) );
    }

    reference_position = event->pos();

    update_view();

    updateGL();
}


void environment_view::mousePressEvent ( QMouseEvent * event )
{
    QWidget::mousePressEvent ( event );

    reference_position = event->pos();
}


void environment_view::wheelEvent ( QWheelEvent * event )
{
    QWidget::wheelEvent ( event );

    view_distance -= event->angleDelta().y();
    view_distance = qMax ( view_distance, 50.0f );

    update_view();

    updateGL();
}


void environment_view::set_environment ( const ues::env::environment & new_environment )
{
    env.reset ( new ues::gui::gl::environment ( new_environment ) );
}


QImage environment_view::save_to_image ( const QSize & size )
{
    QOpenGLFramebufferObject framebuffer ( size, QOpenGLFramebufferObject::Depth );

    // Replace default framebuffer with framebuffer object.
    resizeGL ( size.width(), size.height() );
    framebuffer.bind();

    // Render to framebuffer object.
    updateGL();
    QImage result = framebuffer.toImage();

    // Restore default framebuffer.
    resizeGL ( width(), height() );
    framebuffer.release();

    return result;
}
