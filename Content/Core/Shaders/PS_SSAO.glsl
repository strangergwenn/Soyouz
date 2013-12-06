#version 120
varying vec2 vUv0;

uniform sampler2D sSceneDepthSampler;
uniform sampler2D sRotSampler4x4;

uniform vec4 cViewportSize;
uniform float cFov;
uniform float farClipDistance;
uniform float nearClipDistance;
uniform float cSampleLengthScreenSpace;
uniform float cOffsetScale;
uniform float cDefaultAccessibility;
uniform float cEdgeHighlight;

void main()
{
	const int nSampleNum = 12;

	float clipDepth = farClipDistance - nearClipDistance;

	float fragmentWorldDepth = texture2D(sSceneDepthSampler, vUv0).w * clipDepth;

	vec2 rotationTC = vUv0 * cViewportSize.xy / 4.0;
	vec3 rotationVector = 2.0 * texture2D(sRotSampler4x4, rotationTC).xyz - 1.0;
    
	float r = tan(cSampleLengthScreenSpace * cFov) * fragmentWorldDepth;

	float sampleLength = cOffsetScale;
	float sampleLengthStep = pow((cSampleLengthScreenSpace / sampleLength), 1.0f/nSampleNum);
    
	float accessibility = 0;

	for (int i = 0; i < (nSampleNum/8); i++)
	{
		for (int x = -1; x <= 1; x += 2)
		for (int y = -1; y <= 1; y += 2)
		for (int z = -1; z <= 1; z += 2)
		{
			vec3 offset = normalize(vec3(x, y, z)) * sampleLength;
            
			sampleLength *= sampleLengthStep;
        
			vec3 rotatedOffset = reflect(offset, rotationVector);
                    
			vec2 sampleTC = vUv0 + rotatedOffset.xy * cSampleLengthScreenSpace;
                
			float sampleWorldDepth = texture2D(sSceneDepthSampler, sampleTC).w * clipDepth;
            
			float fRangeIsInvalid = clamp((fragmentWorldDepth - sampleWorldDepth) / r, 0.0, 1.0);
			
			accessibility += mix(float(sampleWorldDepth > (fragmentWorldDepth + rotatedOffset.z * r)) , cDefaultAccessibility, fRangeIsInvalid);
		}
	}
    
	accessibility /= nSampleNum;
	accessibility *= cEdgeHighlight;
	gl_FragColor = vec4(accessibility, accessibility, accessibility, 1.0);
}
