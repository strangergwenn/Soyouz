#version 150

uniform samplerCube envmap;
uniform sampler2D	normalmap;

in vec4 vUv0;
in vec3 vEyeDir;
in vec3 vNormal;
in vec3 vTSLightDir;
in vec3 vTSHalfAngle;

out vec4 pPixel;

void main()
{
	vec3 lightVec = normalize(vTSLightDir);
	vec3 halfAngle = normalize(vTSHalfAngle);

	vec3 normalColor = texture(normalmap, vUv0.xy).xyz;
	vec3 specVec = reflect(vEyeDir, vNormal + normalColor / 5);

	vec4 reflectedColor = texture(envmap, specVec);

	pPixel = reflectedColor;
}
