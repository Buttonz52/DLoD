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

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform sampler2D sampler;
void main(void)
{
	float ka = 0.2; 
	//Simple Phong/toon shading
	vec3 R = reflect(-L,N);
	float ke = 5.f;
	float ks = 1.f;
    // write colour output without modification
	float diffuse = max( 0.0, dot( N, normalize(L - P)));
    vec3 specular = ks * pow(max(dot(R, V), 0.0), ke) * vec3(1.f);

    FragmentColour = vec4(Colour*vec3(diffuse) + vec3(ka) + specular, 1.0);

}
