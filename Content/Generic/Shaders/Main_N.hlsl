/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/


/*----------------------------------------------
	Deefinitions
----------------------------------------------*/

// Vertex output
struct VS_OUTPUT {
	float4 pos:			POSITION;
	float2 coords:		TEXCOORD0;
	float3 position:	TEXCOORD1;
	float3 lightDir:	TEXCOORD3;
	float3 halfAngle:	TEXCOORD4;
};

// Normal map
sampler normalMap: register(s0);

// Vertex parameters
float4   mLightPosition;
float4x4 mWorldViewProj;
float4x4 mWorldView;
float4x4 mView;

// Pixel parameters
float4   mLightColorDiffuse;
float4   mLightColorSpecular;
float    mShininess;


/*----------------------------------------------
	Shaders
----------------------------------------------*/

/**
 * @brief Vertex shader
 * @param pos			World position of the vertex
 * @param normal		Normal vector of the vertex
 * @param coords		Coordinates of the pixel to compute
 * @param tangent		
 * @param binormal		
 * @return Vertex structure
 **/
VS_OUTPUT vertex(
	float4 pos:			POSITION,
	float3 normal:		NORMAL,
	float2 coords:		TEXCOORD0,
	float3 tangent:		TANGENT,
	float3 binormal :	BINORMAL
){
	VS_OUTPUT result;

	float4 position   = mul(mWorldView, pos);
	float3 nnormal    = normalize(mul(mWorldView, normal));
	float3 ntangent   = normalize(mul(mWorldView, tangent));
	float3 nbinormal  = normalize(mul(mWorldView, binormal));
	float3x3 nMatrix  = float3x3(ntangent, nbinormal, nnormal);
	
	result.pos        = mul(mWorldViewProj, pos);
	result.coords     = coords;
	result.position   = pos.xyz / pos.w;
	result.lightDir   = normalize(mul(nMatrix, mul(mView, mLightPosition) - pos));
	result.halfAngle  = normalize(mul(nMatrix, -1.0f * pos + result.lightDir));

	return result;
}



/**
 * @brief Pixel shader
 * @param coords		Coordinates of the pixel to compute
 * @param pos			World position of the vertex
 * @param normal		Normal vector of the vertex
 * @param lightDir		Light direction
 * @param halfAngle		Impact angle
 * @return RGBA computed pixel
 **/
float4 pixel(
	float4 coords:		TEXCOORD0,
	float3 pos:			TEXCOORD1,
	float3 normal:		TEXCOORD2,
	float3 lightDir:	TEXCOORD3,
	float3 halfAngle:	TEXCOORD4
): COLOR0
{
	float4 result;

	float3 px = normalize(tex2D(normalMap, coords).xyz);
	float4 lightFactors = lit(dot(lightDir, px), dot(halfAngle, px), mShininess);
	result = mLightColorDiffuse * lightFactors.y + mLightColorSpecular * lightFactors.z;
	
	return result;
}
