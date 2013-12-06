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

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform sampler2D GlowMap;
uniform float cFarDistance;


/*-------------------------------------------------
	Input / Outputs
/*-----------------------------------------------*/

in vec3 oViewPos;
in vec3 oNormal;
in vec3 oTangent;
in vec3 oBiNormal;
in vec2 oUv0;

out vec4 fragData[3];


/*-------------------------------------------------
	Helpers
/*-----------------------------------------------*/

vec3 expand(vec3 v)
{
   return (v - 0.5) * 2.0;
}


/*-------------------------------------------------
    GBuffer format : 
		R			G			B			A
 0  <          diffuse map          > < specular map >
 1  <          normal map           > < depth buffer >
 2  <            glow map           > 
/*-----------------------------------------------*/

void main()
{
	fragData[0].rgb = texture(DiffuseMap, oUv0).rgb;
	fragData[0].a = length(texture(SpecularMap, oUv0).rgb);
	
	vec3 texNormal = texture(NormalMap, oUv0).rgb;
	texNormal.b = texNormal.b * 2;
	mat3 normalRotation = mat3(oTangent, oBiNormal, oNormal);
	vec3 localTexNormal = normalRotation * texNormal;

	fragData[1].rgb = normalize(localTexNormal);
	fragData[1].a = length(oViewPos) / cFarDistance;
	
	fragData[2].rgb = texture(GlowMap, oUv0).rgb;
	fragData[2].a = 0;
}
