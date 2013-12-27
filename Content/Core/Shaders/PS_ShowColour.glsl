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

uniform sampler2D Tex0;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec2 oUv0;

out vec4 fragColour;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	fragColour = texture(Tex0, oUv0);
}
