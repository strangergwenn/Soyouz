#version 150

uniform vec4 LightPosition;
uniform vec3 EyePosition;
uniform mat4 Projection;

in vec4 vertex;
in vec3 normal;
in vec3 tangent;
in vec4 uv0;

out vec4 vUv0;
out vec3 vEyeDir;
out vec3 vNormal;
out vec3 vTSLightDir;
out vec3 vTSHalfAngle;

void main()
{
	gl_Position = Projection * vertex;
	vec3 lightDir = normalize(LightPosition.xyz - (vertex * LightPosition.w).xyz);
	vec3 binormal = cross(normal, tangent);
	
	mat3 rotation = mat3(vec3(tangent[0], binormal[0], normal[0]),
						vec3(tangent[1], binormal[1], normal[1]),
						vec3(tangent[2], binormal[2], normal[2]));
	
	vec3 eyeDir = normalize(EyePosition - vertex.xyz);
	vec3 halfAngle = normalize(eyeDir + lightDir);
	
	vUv0 = uv0;
	vNormal = normal.xyz;
    vEyeDir = -eyeDir;
	vTSLightDir = rotation * lightDir;
	vTSHalfAngle = rotation * halfAngle;
}
