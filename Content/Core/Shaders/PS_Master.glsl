#version 150

uniform vec4 LightDiffuse;
uniform vec4 LightSpecular;

uniform sampler2D	diffuseMap;
uniform sampler2D	normalMap;
uniform sampler2D	specMap;
uniform samplerCube envMap;

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

	vec3 spec = texture(specMap, vUv0.xy).xyz;
	vec3 normal = texture(normalMap, vUv0.xy).xyz;
	vec4 color = vec4(texture(diffuseMap, vUv0.xy).xyz, 1);
	
	vec3 specVec = reflect(vEyeDir, vNormal);
	vec4 specColor = texture(envMap, specVec);

	float specGray = (spec.r + spec.g + spec.b) / 3.0;
	float specFactor = pow(clamp(dot(normal, halfAngle), 0.0, 1.0), 4.0);

	pPixel = 1 * color;
	pPixel += 0.1 * LightDiffuse * clamp(dot(normal, lightVec), 0.0, 1.0);
	pPixel += LightSpecular * specFactor * specGray * specColor;
	pPixel.w = 1.0;
}
