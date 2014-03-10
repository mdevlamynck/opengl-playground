#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 uv;

uniform mat4 modelViewProjection;
uniform mat4 modelView;
uniform vec4 lightColor;
uniform vec4 lightPosition;
uniform vec4 ambiantLightColor;

smooth out vec4 fragColor;

void main()
{
	gl_Position					= modelViewProjection * vec4(position, 1.0);

	vec4	normalLightPosition	= normalize( lightPosition - (modelView * vec4(position, 1.0))	);
	float	incidenceAngle		= clamp( dot( modelView * vec4(normal, 0.0), normalLightPosition	), 0, 1	);
	fragColor					= (lightColor * color * incidenceAngle) + (ambiantLightColor * color);
}
