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

out vec4 gBuffer[4];

/*-------------------------------------------------
	Helpers
/*-----------------------------------------------*/

vec3 packFloat(float f)
{
    vec3 color;
    color.b = floor(f / 256.0 / 256.0);
    color.g = floor((f - color.b * 256.0 * 256.0) / 256.0);
    color.r = floor(f - color.b * 256.0 * 256.0 - color.g * 256.0);
    return color / 256.0;
}

/*
 +-----------------------------------------------------------+
 | GBuffer format as three 32 bits textures (12 bytes per px)|
 +---+-------------+-------------+-------------+-------------+
 |MRT|      R      |      G       |      B      |      A     |
 +---+-------------+-------------+-------------+-------------+
 | 0 |  diffuse R  |  diffuse G   |  diffuse B  |  specular  |
 | 1 |  normal R   |   normal G   |   normal B  |  |
 | 2 |    glow R   |    glow G    |    glow B   |  |
 | 2 |   depth R   |    depth G   |   depth B   |  |
 +---+-------------+-------------+-------------+-------------+
*/

void main()
{
	// Specific planet factor (temporary rim lighting)
	float rimFactor = (length(oViewPos) - 800000) / 500000;
	if (rimFactor < 0)
	{
		rimFactor = 0;
	}

	// Diffuse + specularity
	if (length(cDiffuseColor) > 0.01)
	{
		gBuffer[0].rgb = cDiffuseColor;
	}
	else
	{
		gBuffer[0].rgb = texture(DiffuseMap, oUv0).rgb;
		gBuffer[0].rgb += vec3(0.1, 0.5, 0.95) * rimFactor;
	}
	gBuffer[0].a = length(texture(SpecularMap, oUv0).rgb);
	
	// Normal mapping setup
	vec3 texNormal = texture(NormalMap, oUv0).rgb;
	mat3 normalRotation = mat3(oTangent, oBiNormal, oNormal);
	vec3 localTexNormal = normalRotation * texNormal;

	float multiplier = 65535.0 / cFarDistance;
	uint depth = uint(clamp(length(oViewPos) * multiplier, 0.0, 65535.0));
	float hDepth = float(depth) / 65535.0;


	// Normal mapping + depth
	gBuffer[1].rgb = normalize(localTexNormal);
	gBuffer[1].a = hDepth;
	
	// Glow
	vec3 base = texture(GlowMap, oUv0).rgb;
	if (length(cGlowColor) > 0)
	{
		base = cGlowColor * length(base);
	}
	gBuffer[2].rgb = base * cGlowAlpha;
	gBuffer[2].a = 0;

	gBuffer[3].rgb = packFloat(length(oViewPos));
}
