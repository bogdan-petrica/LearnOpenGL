#version 330 core
in vec3 ourColor;
out vec4 color;

void main()
{
    color.x = clamp( ourColor.x, 0.0, 1.0 );
	color.y = clamp( ourColor.y, 0.0, 1.0 );
	color.z = ourColor.z;
	color.w = 1.0f;
}