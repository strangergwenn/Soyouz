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

uniform vec4		invTexSize;

uniform sampler2D	scene;
uniform sampler2D	blurdata;


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
	float blurSize = invTexSize.y;

	pPixel += texture(blurdata, vec2(vUv0.x - 4.0*blurSize, vUv0.y)) * 1.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x - 3.0*blurSize, vUv0.y)) * 2.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x - 2.0*blurSize, vUv0.y)) * 3.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x - blurSize, vUv0.y)) * 4.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y)) * 5.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x + blurSize, vUv0.y)) * 4.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x + 2.0*blurSize, vUv0.y)) * 3.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x + 3.0*blurSize, vUv0.y)) * 2.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x + 4.0*blurSize, vUv0.y)) * 1.0/25.0;
 
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y - 4.0*blurSize)) * 1.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y - 3.0*blurSize)) * 2.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y - 2.0*blurSize)) * 3.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y - blurSize)) * 4.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y)) * 5.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y + blurSize)) * 4.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y + 2.0*blurSize)) * 3.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y + 3.0*blurSize)) * 2.0/25.0;
	pPixel += texture(blurdata, vec2(vUv0.x, vUv0.y + 4.0*blurSize)) * 1.0/25.0;

	pPixel *= 4;
	pPixel += texture(scene, vUv0.xy);
}