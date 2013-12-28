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
#define KERNEL_SIZE 12

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
	for (int x = -KERNEL_SIZE / 2; x < KERNEL_SIZE / 2; x++)
	for (int y = -KERNEL_SIZE / 2; y < KERNEL_SIZE / 2; y++)
	{
		color += texture2D(input1, vec2(vUv0.x + x * size.z, vUv0.y + y * size.w)).x;
	}
	color /= (KERNEL_SIZE * KERNEL_SIZE);
	pPixel = vec4(color, color, color, 1);
}
