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

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void)
{

	FragmentColour = texture(sampler, uv.xy);
}
