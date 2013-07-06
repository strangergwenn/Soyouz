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
   float4 pos:		POSITION;
   float2 coords:	TEXCOORD0;
};

// Maps
sampler diffuseMap: register(s0);
sampler specularMap: register(s1);

// Parameters
float mSpecularIntensity;
float mSpecularPower;


/*----------------------------------------------
	Shaders
----------------------------------------------*/

/**
 * @brief Vertex shader
 * @param pos			World position of the vertex
 * @param coords		Coordinates of the pixel to compute
 * @return Vertex structure
 **/
VS_OUTPUT vertex(
    float4 pos:		POSITION, 
    float2 coords:	TEXCOORD0,
	uniform float4x4 mWorldViewProj
){
   VS_OUTPUT data;
 
   data.pos = mul(mWorldViewProj, pos);
   data.coords = coords;
 
   return data;
}


/**
 * @brief Pixel shader
 * @param coords		Coordinates of the pixel to compute
 * @return RGBA computed pixel
 **/
float4 pixel(float4 coords: TEXCOORD0): COLOR0
{
    float4 pxDiff = tex2D(diffuseMap, coords);
	float4 pxSpec = tex2D(specularMap, coords);
	float4 result = pxDiff + mSpecularIntensity * pow(pxSpec, mSpecularPower);
	return result;
}
