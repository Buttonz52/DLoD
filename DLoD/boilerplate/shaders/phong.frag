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
	//Simple Phong/toon shading
	vec3 R = reflect(-L,N);
	float ka = 0.5;
	float kd = 0.5
	float ke = 5.0f;
	float ks = 1.0f;
	
	vec3 ambient = ka * Colour;

    // write colour output without modification
	float diffuse = kd * max( 0.0, dot( N, normalize(L - P)));

    vec3 specular = ks * pow(max(dot(R, V), 0.0), ke) * vec3(1.f);

    FragmentColour = vec4(ambient + vec3(diffuse) + specular, 1.0);

}
