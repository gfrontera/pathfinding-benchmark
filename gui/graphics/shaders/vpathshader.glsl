uniform mat4 u_mvp_matrix;
uniform vec3 u_color;

attribute vec4 a_position;
attribute vec3 a_normal;

varying vec4 v_color;

void main()
{
    v_color = vec4( u_color, 1 );
    
    // Calculate vertex position in screen space.
    gl_Position = u_mvp_matrix * a_position;
}
