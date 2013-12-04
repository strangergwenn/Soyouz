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

uniform sampler2D input1;
uniform vec4 size;

in vec2 vUv0;

out vec4 pPixel;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	float color = 0;
	for (int x = -2; x < 2; x++)
	for (int y = -2; y < 2; y++)
	{
		color += texture2D(input1, vec2(vUv0.x + x * size.z, vUv0.y + y * size.w)).x;
	}
	color /= 16;
	pPixel = vec4(color, color, color, 1);
}
