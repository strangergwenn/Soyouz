struct VS_RESULT {
	float4 pos:       POSITION;
	float2 texCoord:  TEXCOORD0;
	float3 position:  TEXCOORD1;
	float3 lightDir:  TEXCOORD3;
	float3 halfAngle: TEXCOORD4;
};

uniform float4   lightPosition;
uniform float4x4 worldViewProj;
uniform float4x4 worldView;
uniform float4x4 view;

VS_RESULT main(
	float4 pos: POSITION,
	float3 normal: NORMAL,
	float2 texCoord0: TEXCOORD0,
	float3 tangent: TANGENT,
	float3 binormal : BINORMAL
){
	VS_RESULT result;

	float4 position   = mul(worldView, pos);

	result.pos        = mul(worldViewProj, pos);
	result.texCoord   = texCoord0;
	result.position   = position.xyz / position.w;

	float3 nnormal    = normalize(mul(worldView, normal));
	float3 ntangent   = normalize(mul(worldView, tangent));
	float3 nbinormal  = normalize(mul(worldView, binormal));

	float3x3 normalMatrix = float3x3(ntangent, nbinormal, nnormal);

	result.lightDir   = normalize(mul(normalMatrix, mul(view, lightPosition) - position));
	result.halfAngle  = normalize(mul(normalMatrix, -1.0f * position + result.lightDir));

	return result;
}
