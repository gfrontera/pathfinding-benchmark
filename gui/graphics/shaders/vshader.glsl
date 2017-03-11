uniform mat4 u_mvp_matrix;
uniform vec3 u_light_vector;

attribute vec4 a_position;
attribute vec3 a_normal;

varying vec4 v_color;

void main()
{
    float diffuse = max( dot( a_normal, -u_light_vector ), 0.2 );
    
    v_color = vec4( vec3( diffuse ), 1 );
    
    // Calculate vertex position in screen space.
    gl_Position = u_mvp_matrix * a_position;
}
