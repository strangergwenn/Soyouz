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

uniform sampler2D	input1;
uniform sampler2D	input2;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec4 vUv0;

out vec4 pPixel;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	vec4 pixel1 = texture(input1, vec2(vUv0.x, vUv0.y));
	vec4 pixel2 = texture(input2, vec2(vUv0.x, vUv0.y));
	pPixel = pixel1 + pixel2;
}
