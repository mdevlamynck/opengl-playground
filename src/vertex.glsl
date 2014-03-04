#version 330

// 0 = Position
// 1 = Color
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

uniform mat4 viewProj;
uniform mat4 transform;

smooth out vec4 fragColor;

void main()
{
    gl_Position	= viewProj * transform * position;
	fragColor	= color;
}
