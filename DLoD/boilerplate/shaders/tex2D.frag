// ==========================================================================
// Fragment program for 2d textures
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;
in vec3 N;
in vec3 L;
in vec3 P;
in vec3 V;
in vec3 uv;
uniform sampler2D sampler;
uniform sampler2D bump;
// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform int hasBumpTexture;

void main(void)
{
	vec4 imageColour = texture(sampler,uv.xy);
	float diffuse;

	diffuse = max(0.0, dot(N, normalize(L-P)));

	FragmentColour = imageColour + vec4(vec3(diffuse*0.5),1.0);
	//FragmentColour= imageColour;
}


