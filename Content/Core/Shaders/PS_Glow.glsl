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

uniform float		baseStrength;
uniform float		basePower;
uniform vec4		baseAlpha;

uniform float		panningX;
uniform float		panningY;
uniform float		time;

uniform vec3		baseColor;
uniform vec3		lightColor;

uniform sampler2D	baseMap;
uniform sampler2D	panningMap;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec4 vUv0;

out vec4 pPixel;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	vec2 coords = vec2(vUv0.x - time * panningX, vUv0.y + time * panningY);
	vec4 panningData = texture(panningMap, coords);
	vec4 baseData = texture(baseMap, vUv0.xy);

	float alpha = baseData.w;
	vec4 basePowerData = pow(baseData, basePower * vec4(1.0, 1.0, 1.0, 1.0));
	vec4 light = vec4(lightColor, alpha) * basePowerData;
	vec4 base = vec4(baseColor, 1 - alpha) * (1 - basePowerData);

	pPixel = (base + light) * panningData * baseStrength;
	pPixel.w = alpha * baseAlpha.x;
}