#version 330 core

in vec3 vertexColor;
in vec2 vertexTextCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float offset;

#define pi 3.141592653589793238462643383279

void main()
{
	//color = vec4(vertexColor, 1.0f);
	vec2 xx;
	float dx = vertexTextCoord.x - 0.5;
	float dy = vertexTextCoord.y - 0.5;
	xx.x = atan(dy, dx) / (2 * pi);
	xx.y = sqrt(dx * dx + dy * dy) + offset;
	color = mix(texture(ourTexture1,xx), texture(ourTexture2, vec2(vertexTextCoord.x, 1.0-vertexTextCoord.y)), 0.2);
}
