#version 150

uniform vec4 lightPosition; // object space
uniform mat4 worldViewProj;
uniform mat4 worldMatrix;
uniform mat4 texViewProj;

in vec4 vertex;
in vec3 normal;
in vec3 tangent;
in vec4 uv0;

out vec4 uvproj;
out vec4 oUv0;
out vec3 oTSLightDir;

void main()
{
	gl_Position = worldViewProj * vertex;
	vec3 lightDir = lightPosition.xyz - (vertex * lightPosition.w).xyz;
	vec3 binormal = cross(normal, tangent);
	
	mat3 rotation = mat3(vec3(tangent[0], binormal[0], normal[0]),
						vec3(tangent[1], binormal[1], normal[1]),
						vec3(tangent[2], binormal[2], normal[2]));
	
	oUv0 = uv0;
	oTSLightDir = rotation * lightDir;
	uvproj = texViewProj * (worldMatrix * vertex);
}
