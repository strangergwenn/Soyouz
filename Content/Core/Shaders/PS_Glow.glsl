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

uniform float		emissiveStrength;
uniform float		panningX;
uniform float		panningY;
uniform float		time;

uniform sampler2D	emissiveMap;


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
	vec2 coords = vec2(vUv0.x - time * panningX, vUv0.y + time * panningY);
	vec4 emissiveData = texture(emissiveMap, coords);
	pPixel = emissiveData * emissiveStrength;
}
