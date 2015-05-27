#version 330 core

in vec3 interpolatedColor;
in vec2 interpolatedTextCoords;

out vec4 color;

uniform float globalArg;
uniform sampler2D crateTextureUnit;
uniform sampler2D smileTextureUnit;

void main()
{
    vec4 crateColor = texture(crateTextureUnit, interpolatedTextCoords);
    float pieces = 0.33f;
    vec4 smileColor = texture(smileTextureUnit, vec2(interpolatedTextCoords.x/pieces,
        (1.0f - interpolatedTextCoords.y)/pieces));
	color = mix(crateColor * vec4(interpolatedColor, 1.0), smileColor, 1.0f - globalArg);
}