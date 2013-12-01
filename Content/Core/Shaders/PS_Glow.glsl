/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

/*-------------------------------------------------
	Config
/*-----------------------------------------------*/

#version 440

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

out vec4 fragData[3];


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

	fragData[0] = vec4(0, 0, 0, 0);
	fragData[1] = vec4(0, 0, 0, 0);
	fragData[2] = (base + light) * panningData * baseStrength * baseAlpha.x;
	fragData[2].w = alpha * baseAlpha.x;
}
