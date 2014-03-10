#version 330

smooth in vec3 fragPosition;
smooth in vec3 fragNormal;
smooth in vec4 fragColor;

uniform mat4 modelView;
uniform vec4 lightColor;
uniform vec4 lightPosition;
uniform vec4 ambiantLightColor;

out vec4 outputColor;

void main()
{
	vec4	normalLightPosition	= normalize( lightPosition - (modelView * vec4(fragPosition, 1.0))	);
	float	incidenceAngle		= clamp( dot( modelView * vec4(fragNormal, 0.0), normalLightPosition	), 0, 1	);

	outputColor	= (lightColor * fragColor * incidenceAngle) + (ambiantLightColor * fragColor);
}
