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

uniform vec4		textureColor;

uniform sampler2D	textureMap;


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
	vec4 data = texture(textureMap, vUv0.xy);
	pPixel = data * textureColor;
}
