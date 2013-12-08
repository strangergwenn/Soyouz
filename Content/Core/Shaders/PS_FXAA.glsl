/**
* GLSL source from the Processing library, released under LGPL
* See : http://code.google.com/p/processing/source/browse/trunk/processing/
* FXAA shader, GLSL code adapted from http://horde3d.org/wiki/index.php5?title=Shading_Technique_-_FXAA
**/

/*-------------------------------------------------
	Config
/*-----------------------------------------------*/

#version 150

uniform sampler2D textureSampler;
uniform vec2 texcoordOffset;


/*-------------------------------------------------
	Input / Outputs
/*-----------------------------------------------*/

in vec4 vUv0;

out vec4 pPixel;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	// Definitions
	float FXAA_SPAN_MAX = 8.0;
	float FXAA_REDUCE_MUL = 1.0/4.0;
	float FXAA_REDUCE_MIN = (1.0/256.0);

	// Samples
	vec3 rgbNW = texture2D(textureSampler, vUv0.xy + (vec2(-1.0, -1.0) / texcoordOffset)).xyz;
	vec3 rgbNE = texture2D(textureSampler, vUv0.xy + (vec2(+1.0, -1.0) / texcoordOffset)).xyz;
	vec3 rgbSW = texture2D(textureSampler, vUv0.xy + (vec2(-1.0, +1.0) / texcoordOffset)).xyz;
	vec3 rgbSE = texture2D(textureSampler, vUv0.xy + (vec2(+1.0, +1.0) / texcoordOffset)).xyz;
	vec3 rgbM = texture2D(textureSampler, vUv0.xy).xyz;

	// Parameters
	vec3 luma = vec3(0.299, 0.587, 0.114);
	float lumaNW = dot(rgbNW, luma);
	float lumaNE = dot(rgbNE, luma);
	float lumaSW = dot(rgbSW, luma);
	float lumaSE = dot(rgbSE, luma);
	float lumaM = dot( rgbM, luma);
	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

	// Direction
	vec2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

	// Settings
	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
	float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
	dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
	max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) / texcoordOffset;

	// Output calculation
	vec3 rgbA = (1.0/2.0) * (
		texture2D(textureSampler, vUv0.xy + dir * (1.0/3.0 - 0.5)).xyz +
		texture2D(textureSampler, vUv0.xy + dir * (2.0/3.0 - 0.5)).xyz);
	vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
		texture2D(textureSampler, vUv0.xy + dir * (0.0/3.0 - 0.5)).xyz +
		texture2D(textureSampler, vUv0.xy + dir * (3.0/3.0 - 0.5)).xyz);
	float lumaB = dot(rgbB, luma);

	// Output
	if ((lumaB < lumaMin) || (lumaB > lumaMax))
	{
		pPixel.xyz = rgbA;
	}
	else
	{
		pPixel.xyz = rgbB;
	}
	pPixel.a = 1.0;
}
