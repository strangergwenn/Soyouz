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

uniform vec3 cDiffuseColor;
uniform vec3 cGlowColor;
uniform float cGlowAlpha;

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
    GBuffer format : 
		R			G			B			A
 0  <          diffuse map          > < specular map >
 1  <          normal map           > < depth buffer >
 2  <            glow map           > 
/*-----------------------------------------------*/

void main()
{
	// Specific planet factor (temporary rim lighting)
	float rimFactor = (length(oViewPos) - 800000) / 500000;
	if (rimFactor < 0)
	{
		rimFactor = 0;
	}

	// Diffuse + specularity
	fragData[0].rgb = cDiffuseColor * texture(DiffuseMap, oUv0).rgb;
	fragData[0].rgb += vec3(0.1, 0.5, 0.95) * rimFactor;
	fragData[0].a = length(texture(SpecularMap, oUv0).rgb);
	
	// Normal mapping setup
	vec3 texNormal = texture(NormalMap, oUv0).rgb;
	texNormal.b = texNormal.b * 2;
	mat3 normalRotation = mat3(oTangent, oBiNormal, oNormal);
	vec3 localTexNormal = normalRotation * texNormal;

	// Normal mapping + depth
	fragData[1].rgb = normalize(localTexNormal);
	fragData[1].a = length(oViewPos) / cFarDistance;
	
	// Glow
	vec3 base = texture(GlowMap, oUv0).rgb;
	if (length(cGlowColor) > 0)
	{
		base = cGlowColor * length(base);
	}
	fragData[2].rgb = base * cGlowAlpha;
	fragData[2].a = 0;
}

