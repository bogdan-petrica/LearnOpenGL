#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inVertexColor;
layout(location = 2) in vec2 inTextCoord;

out vec4 vertexColor;
out vec2 textCoord;

uniform float positionOffset;

void main()
{
	gl_Position = vec4( position.x + positionOffset, position.y, position.z, 1.0f );
	vertexColor = vec4( inVertexColor, 1.0f );
	textCoord = inTextCoord;
}