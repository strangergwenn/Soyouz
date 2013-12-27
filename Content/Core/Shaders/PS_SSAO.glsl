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

uniform sampler2D sSceneDepthSamplerHigh;
uniform sampler2D sSceneDepthSamplerLow;
uniform sampler2D sRotSampler4x4;

uniform vec4 cViewportSize;
uniform float cFov;
uniform float cStrength;
uniform float cOffsetScale;
uniform float cFarDistance;
uniform float cSampleLengthScreenSpace;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec2 vUv0;

out vec4 pPixel;


/*-------------------------------------------------
	Deph lookup using bi-buffer and linear scaling
	See cFarDistance in SSAO material
	See cFarDistance in Master material
/*-----------------------------------------------*/

float getDepth(vec2 coords)
{
	float div = 65535.0 / cFarDistance;
	float hDepth = 256 * texture2D(sSceneDepthSamplerHigh, coords).w;
	float lDepth = texture2D(sSceneDepthSamplerLow, coords).w;
	return ((hDepth + lDepth) / div);
}


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	// Compute rotation
	int nSampleNum = 24;
	float fragmentWorldDepth = getDepth(vUv0);
	vec2 rotationTC = vUv0 * cViewportSize.xy / 4.0;
	vec3 rotationVector = 2.0 * texture2D(sRotSampler4x4, rotationTC).xyz - 1.0;
    
	// Initialize data
	float accessibility = 0;
	float sampleLength = cOffsetScale;
	float r = tan(cSampleLengthScreenSpace * cFov) * fragmentWorldDepth;
	float sampleLengthStep = pow((cSampleLengthScreenSpace / sampleLength), 0.1f / nSampleNum);

	// Loop on samples
    for (int i = 0; i < (nSampleNum / 8); i++)
	{
		for (int x = -1; x <= 1; x += 2)
		for (int y = -1; y <= 1; y += 2)
		for (int z = -1; z <= 1; z += 2)
		{
			// Compute coordinates for depth lookup
			vec3 offset = normalize(vec3(x, y, z)) * sampleLength;
			sampleLength *= sampleLengthStep;
			vec3 rotatedOffset = reflect(offset, rotationVector);
			vec2 sampleTC = vUv0 + rotatedOffset.xy * cSampleLengthScreenSpace;
                    
			// Compute accessibility
			float sampleWorldDepth = getDepth(sampleTC);
			float fRangeIsInvalid = clamp((fragmentWorldDepth - sampleWorldDepth) / r, 0.0, 1.0);
			accessibility += mix(float(sampleWorldDepth > (fragmentWorldDepth + rotatedOffset.z * r)) , 0.0, fRangeIsInvalid);
		}
	}

	accessibility = 1.5 * clamp((accessibility * cStrength) / nSampleNum, 0.0, 0.6);
	pPixel = vec4(accessibility, accessibility, accessibility, 1.0);
}
