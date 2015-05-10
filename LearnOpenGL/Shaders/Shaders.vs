#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inVertexColor;

out vec4 vertexColor;

uniform float positionOffset;

void main()
{
	gl_Position = vec4( position.x + positionOffset, position.y, position.z, 1.0f );
	//vertexColor = vec4( inVertexColor, 1.0f );
	vertexColor = vec4( position, 1.0f );
}