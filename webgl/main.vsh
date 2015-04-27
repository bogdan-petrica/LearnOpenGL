attribute vec4 a_position;
attribute vec4 a_color;
varying vec4 v_color;

void main()
{
    gl_Position = a_position; // Tutorial: gl_Position = vec4(position.x, position.y, position.z, 1.0);
}