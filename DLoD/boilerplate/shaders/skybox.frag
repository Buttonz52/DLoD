// ==========================================================================
// Vertex program for cubemap
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

uniform samplerCube sampler;		//cubemap

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void)
{
	FragmentColour = texture(sampler, uv);		//cubemap
}
