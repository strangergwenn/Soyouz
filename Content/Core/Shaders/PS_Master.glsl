/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

/*-------------------------------------------------
	Config
/*-----------------------------------------------*/

#version 150

uniform float		normalStrength;
uniform float		specularStrength;
uniform float		specularPower;

uniform sampler2D	diffuseMap;
uniform sampler2D	normalMap;
uniform sampler2D	specMap;
uniform sampler2D	glowMap;
uniform samplerCube envMap;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec4 vUv0;
in vec3 vEyeDir;
in vec3 vNormal;
in vec3 vLightDir;
in vec3 vHalfAngle;

out vec4 pPixel;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	vec4 diffuseData = texture(diffuseMap, vUv0.xy);
	vec3 normalData = normalStrength * texture(normalMap, vUv0.xy).xyz;
	vec3 specData = texture(specMap, vUv0.xy).xyz;
	vec3 glowData = texture(glowMap, vUv0.xy).xyz;

	float specFactor = specularStrength * pow(clamp(dot(normalData, vHalfAngle), 0.0, 1.0), specularPower);
	vec3 reflectVec = reflect(-vEyeDir, vNormal + normalData / 5);

	pPixel = diffuseData * clamp(dot(normalData, vLightDir), 0.0, 1.0);
	pPixel += texture(envMap, reflectVec) * specFactor;
}
