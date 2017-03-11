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

#ifndef UES_GUI_ENVIRONMENT_VIEW_H
#define UES_GUI_ENVIRONMENT_VIEW_H

#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>

#include "drawables/environment.h"

namespace Ui
{
class environment_view;
}

namespace ues
{
namespace gui
{

class environment_view : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    /** Constructor method. */
    explicit environment_view ( QWidget * parent = nullptr );
    /** Copy constructor method. */
    environment_view ( const environment_view & ) = delete;

    /** Destructor method. */
    ~environment_view();

    /** Copy assignment operator. */
    environment_view & operator= ( const environment_view & ) = delete;

    void set_environment ( const ues::env::environment & );

    QImage save_to_image ( const QSize & size );

protected:
    /** \name OpenGL functions */
    /** \{ */
    virtual void initializeGL() override;
    virtual void resizeGL ( int width, int height ) override;
    virtual void paintGL() override;
    /** \} */

    virtual void mouseMoveEvent ( QMouseEvent * event ) override;
    virtual void mousePressEvent ( QMouseEvent * event ) override;
    virtual void wheelEvent ( QWheelEvent * event ) override;

    inline const QMatrix4x4 & get_mvp_matrix() const noexcept;

private:
    Ui::environment_view * ui;

    QOpenGLShaderProgram program;
    QMatrix4x4 view, projection;
    QMatrix4x4 mvp;
    QVector3D light_vector;

    QPoint reference_position;
    QVector2D view_target;
    QVector2D view_angles;
    float view_distance;

    std::unique_ptr< ues::gui::gl::environment > env;

    void init_shaders();
    void update_view();
};


// Inlined methods.

const QMatrix4x4 & environment_view::get_mvp_matrix() const noexcept
{
    return mvp;
}


}
}

#endif // UES_GUI_ENVIRONMENT_VIEW_H
