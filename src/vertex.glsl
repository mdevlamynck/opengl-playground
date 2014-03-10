#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 uv;

uniform mat4 modelViewProjection;
uniform mat4 modelView;

smooth out vec4 fragPosition;
smooth out vec3 fragNormal;
smooth out vec4 fragColor;

void main()
{
	gl_Position					= modelViewProjection * vec4(position, 1.0);

	fragPosition				= modelView * vec4(position, 1.0);
	fragNormal					= normal;
	fragColor					= color;
}
