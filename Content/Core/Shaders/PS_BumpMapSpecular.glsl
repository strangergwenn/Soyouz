#version 150

vec3 expand(vec3 v)
{
	return (v - 0.5) * 2.0;
}

uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform sampler2D normalMap;

in vec4 oUv0;
in vec3 oTSLightDir;
in vec3 oTSHalfAngle;

out vec4 fragColour;

void main()
{
	vec3 lightVec = normalize(oTSLightDir);
	vec3 halfAngle = normalize(oTSHalfAngle);
	vec3 bumpVec = expand(texture(normalMap, oUv0.xy).xyz);

	float specFactor = pow(clamp(dot(bumpVec, halfAngle), 0.0, 1.0), 4.0);

	fragColour = (lightDiffuse * clamp(dot(bumpVec, lightVec), 0.0, 1.0)) + 
			(lightSpecular * specFactor);
}
