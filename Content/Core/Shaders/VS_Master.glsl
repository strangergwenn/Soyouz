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

uniform vec4 lightPosition;
uniform vec3 eyePosition;
uniform mat4 projection;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec4 vertex;
in vec3 normal;
in vec3 tangent;
in vec4 uv0;

out vec4 vUv0;
out vec3 vEyeDir;
out vec3 vNormal;
out vec3 vLightDir;
out vec3 vHalfAngle;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	gl_Position = projection * vertex;
	vec3 lightDir = normalize(lightPosition.xyz - (vertex * lightPosition.w).xyz);
	vec3 binormal = cross(normal, tangent);
	
	mat3 rotation = mat3(vec3(tangent[0], binormal[0], normal[0]),
						vec3(tangent[1], binormal[1], normal[1]),
						vec3(tangent[2], binormal[2], normal[2]));
		
	vUv0 = uv0;
	vNormal = normalize(normal.xyz);
    vEyeDir = normalize(eyePosition - vertex.xyz);
	vLightDir = rotation * lightDir;
	vHalfAngle = rotation * (vEyeDir + lightDir);
}
