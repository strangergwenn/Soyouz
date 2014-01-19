/******************************************************************************
Copyright (c) W.J. van der Laan

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software  and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so, subject 
to the following conditions:

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

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;


/*-------------------------------------------------
	Input / Output
/*-----------------------------------------------*/

in vec2 oUv0;
in vec2 projCoord;

out vec4 fragColour;


/*-------------------------------------------------
	Shader
/*-----------------------------------------------*/

void main()
{
	// Upper line
	if (oUv0.r < 0.5)
	{
		// Diffuse
		if (oUv0.g < 0.5)
		{
			fragColour = texture(Tex0, 2 * oUv0);
		}

		// Normal
		else
		{
			fragColour = texture(Tex1, 2 * vec2(oUv0.r, oUv0.g - 0.5));
		}
	}

	// Lower line
	else
	{
		// Spec + depth
		if (oUv0.g < 0.5)
		{
			vec4 depth = texture(Tex1, 2 * vec2(oUv0.r - 0.5, oUv0.g));
			vec4 spec = texture(Tex0, 2 * vec2(oUv0.r - 0.5, oUv0.g));
			fragColour = vec4(spec.w, 0, depth.w, 0);
		}

		// Glow
		else
		{
			fragColour = vec4(texture(Tex1, 2 * vec2(oUv0.r - 0.5, oUv0.g - 0.5)).a);
		}
	}
}
