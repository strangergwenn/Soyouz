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
in vec4 uv0;

out vec4 vUv0;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	vUv0 = uv0;
	gl_Position = projection * vertex;
}
