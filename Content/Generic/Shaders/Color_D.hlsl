/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/


/*----------------------------------------------
	Shaders
----------------------------------------------*/

/**
 * @brief Vertex shader
 * @param pos				World position of the vertex
 * @param mWorldViewProj	World view
 * @return Vertex position
 **/
float4 vertex(float4 pos: POSITION, uniform float4x4 mWorldViewProj) : POSITION
{
	return mul(mWorldViewProj, pos);
}


/**
 * @brief Pixel shader
 * @param mColor		Color to set
 * @return RGBA computed pixel
 **/
float4 pixel(uniform float4 mColor : COLOR): COLOR
{
	return mColor;
}
