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

    float a = 0.75f;
    float r = 2.0f - 2*a;

    float xCoord = a + inTextCoord.x * r;
    float yCoord = a + ( 1.0f - inTextCoord.y ) * r;
	textCoord = vec2( xCoord, yCoord );
}