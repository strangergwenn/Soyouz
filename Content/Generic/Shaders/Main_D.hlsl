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

// Diffuse map
sampler diffuseMap: register(s0);


/*----------------------------------------------
	Shaders
----------------------------------------------*/

/**
 * @brief Vertex shader
 * @param pos			World position of the vertex
 * @param normal		Normal vector of the vertex
 * @param coords		Coordinates of the pixel to compute
 * @return Vertex structure
 **/
VS_OUTPUT vertex(
    float4 pos:		POSITION, 
    float3 normal:	NORMAL,
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
    return tex2D(diffuseMap, coords);
}
