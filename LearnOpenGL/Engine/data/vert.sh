#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textCoord;

out vec3 vertexColor;
out vec2 vertexTextCoord;

void main()
{
	gl_Position = vec4(position, 1.0);
	vertexColor = color;
	vertexTextCoord = textCoord;
}
