#version 150

uniform samplerCube envmap;

in vec4 vCoords;
in vec3 vEyeDir;
in vec3 vNormal;

out vec4 pPixel;

void main()
{
	vec3 normView = normalize(vEyeDir);
	vec3 reflVec = reflect(normView, vNormal);
	pPixel = texture(envmap, reflVec);
}
