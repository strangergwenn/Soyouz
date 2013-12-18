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

// Samplers
uniform sampler2D sSceneDepthSamplerHigh;
uniform sampler2D sSceneDepthSamplerLow;
uniform sampler2D sRotSampler4x4;

// Performance parameters
uniform int cSampleCount;
uniform float cRangeCompression;

// Artistic parameters
uniform float cSampleLengthScreenSpace;
uniform float cOffsetScale;
uniform float cDefaultAccessibility;
uniform float cEdgeHighlight;

// Viewport
uniform float cFov;
uniform vec4 cViewportSize;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec2 vUv0;

out vec4 pPixel;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{	
	// Get depth from bi-buffer
	float hDepth = 256 * texture2D(sSceneDepthSamplerHigh, vUv0).w;
	float lDepth = texture2D(sSceneDepthSamplerLow, vUv0).w;
	float fragmentWorldDepth = ((hDepth + lDepth)) * cRangeCompression;

	// Compute rotation
	vec2 rotationTC = vUv0 * cViewportSize.xy / 4.0;
	vec3 rotationVector = 2.0 * texture2D(sRotSampler4x4, rotationTC).xyz - 1.0;
    
	// Initialize data
	float r = tan(cSampleLengthScreenSpace * cFov) * fragmentWorldDepth;
	float sampleLength = cOffsetScale;
	float sampleLengthStep = pow((cSampleLengthScreenSpace / sampleLength), 1.0f/cSampleCount);
	float accessibility = 0;

	// Loop on samples
	for (int i = 0; i < (cSampleCount / 8); i++)
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
            
			// Get depth from bi-buffer
			float hSDepth = 256 * texture2D(sSceneDepthSamplerHigh, sampleTC).w;
			float lSDepth = texture2D(sSceneDepthSamplerLow, sampleTC).w;
			float sampleWorldDepth = (hSDepth + lSDepth) * cRangeCompression;
            
			// Compute accessibility
			float fRangeIsInvalid = clamp((fragmentWorldDepth - sampleWorldDepth) / r, 0.0, 1.0);
			accessibility += mix(float(sampleWorldDepth > (fragmentWorldDepth + rotatedOffset.z * r)) , cDefaultAccessibility, fRangeIsInvalid);
		}
	}
    
	// Final setup
	accessibility /= cSampleCount;
	accessibility *= cEdgeHighlight;
	pPixel = vec4(accessibility, accessibility, accessibility, 1.0);
}
