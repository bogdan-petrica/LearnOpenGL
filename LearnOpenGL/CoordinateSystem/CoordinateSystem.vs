#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inVertexColor;
layout(location = 2) in vec2 inTextCoord;

out vec4 vertexColor;
out vec2 textCoord;

uniform mat4 model;     // world transform
uniform mat4 view;      // camera transform
uniform mat4 proj;      // clip transform

void main()
{
	gl_Position = proj * view * model * vec4( position.x, position.y, position.z, 1.0f );
	vertexColor = vec4( inVertexColor, 1.0f );
	
    float a = 0.0f;
    float r = 1.0f - a;
	
    float xCoord = a + inTextCoord.x * r;
    
    // reverse the Y text coordinate
    // as the bottom left in OpenGL is ( 0, 0 )
    // while in BMP bottom left is ( 0, Height )
    float yCoord = a + ( 1.0f - inTextCoord.y ) * r;
	textCoord = vec2( xCoord, yCoord );
}