#version 330 core

in vec3 interpolatedColor;

out vec4 color;

uniform float arg;

void main()
{
	color = vec4(interpolatedColor * arg, 1.0f);
}