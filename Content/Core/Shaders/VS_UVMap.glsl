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

uniform mat4 projection;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec4 vertex;
in vec2 uv0;

out vec2 vUv0;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	vUv0 = uv0;
	gl_Position = projection * vertex;
}
