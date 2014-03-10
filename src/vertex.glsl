#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 uv;

uniform mat4 modelViewProjection;

smooth out vec3 fragPosition;
smooth out vec3 fragNormal;
smooth out vec4 fragColor;

void main()
{
	gl_Position					= modelViewProjection * vec4(position, 1.0);

	fragPosition				= position;
	fragNormal					= normal;
	fragColor					= color;
}
