#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textCoords;

out vec3 interpolatedColor;
out vec2 interpolatedTextCoords;

uniform float uGlobalVariationVal;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;

float
map(float v, float fB, float fE, float sB, float sE)
{
  return sB + ((v - fB)*((sE - sB)/(fE - fB)));
}

void main()
{
  vec3 pos = map(uGlobalVariationVal, 0.0f, 1.0f, 0.9f, 1.0f) * position.xyz;
  gl_Position = uViewMatrix * uModelMatrix * vec4(pos, 1.0f);
  interpolatedColor = color;
  interpolatedTextCoords = textCoords;
}