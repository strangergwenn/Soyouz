sampler normalTexture: register(s0);

uniform float4   lightColorDiffuse;
uniform float4   lightColorSpecular;
uniform float    shininess;

float4 main(
	float4 texCoord:  TEXCOORD0,

	float3 pos:       TEXCOORD1,
	float3 normal:    TEXCOORD2,
	float3 lightDir:  TEXCOORD3,
	float3 halfAngle: TEXCOORD4
) : COLOR0
{
	float4 result;

	float3 normalFromTexture = normalize((tex2D(normalTexture, texCoord).xyz - float3(0.5f, 0.5f, 0.5f)));
	float4 lightFactors = lit(dot(lightDir, normalFromTexture), dot(halfAngle, normalFromTexture), shininess);

	result = lightColorDiffuse * lightFactors.y + lightColorSpecular * lightFactors.z;

	return result;
}
