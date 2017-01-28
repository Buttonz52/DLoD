// ==========================================================================
// Vertex program for barebones GLFW boilerplate
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
	//Simple Phong/toon shading
	vec3 R = reflect(-L,N);
	float ke = 5.f;
	float ks = 1.f;
    // write colour output without modification
	float diffuse = max( 0.0, dot( N, normalize(L - P)));
    vec3 specular = ks * pow(max(dot(R, V), 0.0), ke) * vec3(1.f);

	//using diffuse as factor for toon shading, no reason in particular.
    if (diffuse > 0.9)	{
		diffuse = 1.f;
	} 
	else if (diffuse > 0.6) {
		diffuse = 0.6f;
	}
	else if (diffuse > 0.3) {
		diffuse = 0.3f;
	}
	else {
		diffuse = 0.1f;
	}
	//FragmentColour = vec4(uv.xy, 0.0, 1.0);
	FragmentColour = texture(sampler, uv.xy);
   // FragmentColour = vec4(Colour*vec3(diffuse) + specular, 1.0);
    //FragmentColour = vec4(Colour*vec3(diffuse) + specular, 1.0);

}
