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

uniform float transparency;
uniform samplerCube radiancemap;

uniform float exposure;
uniform float reflectance;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

vec3 FresnelReflectance(vec3 R0, float cosine) {
	return R0 + (vec3(1.f)-R0) * pow(1.0-cos(cosine),5.f);

}

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

	else if (diffuse > 0.4) {
		diffuse = 0.4f;
	}
	else {
		diffuse = 0.3f;
	}
    vec3 toonColour = Colour*vec3(diffuse);

	vec3 v = normalize(V); 
	vec3 r = reflect(-v, N);
	vec4 nits = texture(radiancemap, r); //vec3 for uv, vertex position

	//vec4 lux = texture(irradiancemap, normal);

	float NdotV = dot(N,v);

	vec3 shaded =  vec3(FresnelReflectance(vec3(reflectance), NdotV ) * nits.xyz + toonColour);
	
	FragmentColour = vec4(shaded *exposure, 1.f);	//don't mess with the alphas, that is bad news
}

