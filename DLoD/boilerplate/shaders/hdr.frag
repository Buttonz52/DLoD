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

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform sampler2D sampler;
uniform samplerCube radiancemap;
uniform float exposure;
uniform float reflectance;


vec3 FresnelReflectance(vec3 R0, float cosine) {
	return R0 + (vec3(1.f)-R0) * pow(1.0-cos(cosine),5.f);

}
void main(void)
{
	vec4 imageColour = texture(sampler,uv.xy);

	
	//FragmentColour= vec4(imageColour.xyz,1.f);


	vec3 v = normalize(V); 
	vec3 r = reflect(-v, N);
	vec4 nits = texture(radiancemap, r); //vec3 for uv, vertex position

	float NdotV = dot(N,v);

	vec3 shaded =  vec3(FresnelReflectance(vec3(reflectance), NdotV ) * nits +imageColour.xyz);
	
	//FragmentColour = nits;
	FragmentColour = vec4(shaded *exposure, 1.f);	//don't mess with the alphas, that is bad news
}
