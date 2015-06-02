#version 330 core
in vec4 vertexColor;
in vec2 textCoord;
out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float mixValue;

void main()
{
	vec2 coords = textCoord;
	vec2 happyCoords = vec2(1.0f - coords.x, coords.y);
	color = mix(texture(ourTexture1, coords), texture(ourTexture2, happyCoords), mixValue) /* * vertexColor */;
}