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
//in vec3 lVec;

uniform samplerCube sampler;		//cubemap
//uniform samplerCube normalmap;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void)
{
	vec4 texCol = texture(sampler, uv);		//cubemap

	//use normal map to get normals for image to calculate shading.
 /*  vec3 normal = normalize((texture(normalmap, uv).rgb*2.0-1.0)); 

	//simple diffuse shading.
   float diffuse = max( 0.0, dot( normal, normalize(lVec)));

	vec3 R = reflect(-L,N);
	float ke = 5.f;
	float ks = 1.f;
    // write colour output without modification
    vec3 specular = ks * pow(max(dot(R, V), 0.0), ke) * vec3(1.f);

	FragmentColour = texCol + vec4(vec3(diffuse*0.2), 1.0);*/
	FragmentColour = texCol;
}
