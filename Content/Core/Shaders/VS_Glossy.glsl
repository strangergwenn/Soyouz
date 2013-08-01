#version 150

uniform vec3 EyePosition;
uniform mat4 Projection;

in vec4 vertex;
in vec4 normal;
in vec3 tangent;
in vec4 uv0;

out vec4 vCoords;
out vec3 vEyeDir;
out vec3 vNormal;

void main()
{
	vCoords = uv0;	
	vNormal = normal.xyz;
    vEyeDir = vertex.xyz - EyePosition;
    gl_Position = Projection * vertex;
}
