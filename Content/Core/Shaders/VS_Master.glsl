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


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec4 vertex;
in vec3 normal;
in vec2 uv0;
in vec3 tangent;
in vec3 binormal;

out vec3 oViewPos;
out vec3 oNormal;
out vec3 oTangent;
out vec3 oBiNormal;
out vec2 oUv0;

uniform mat4 cWorldViewProj;
uniform mat4 cWorldView;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	gl_Position = cWorldViewProj * vertex;
	oNormal = (cWorldView * vec4(normal,0)).xyz;
	oTangent = (cWorldView * vec4(tangent,0)).xyz;
	oBiNormal =(cWorldView * vec4(binormal,0)).xyz;
	oViewPos = (cWorldView * vertex).xyz;
	oUv0 = uv0;
}
