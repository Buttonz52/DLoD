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
in vec4 positionLightSpace;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform sampler2D sampler;
uniform samplerCube radiancemap;
uniform sampler2D shadowmap;

uniform float exposure;
uniform float reflectance;

//shadow calculation
//from : https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
float ShadowCalculation(vec4 posLightSpace) {
	vec3 projCoords = posLightSpace.xyz/posLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowmap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow;
}

vec3 FresnelReflectance(vec3 R0, float cosine) {
	return R0 + (vec3(1.f)-R0) * pow(1.0-cos(cosine),5.f);

}
void main(void)
{
	//2D texture
	vec4 imageColour = texture(sampler,uv.xy);

	//get "uv" coordinates for skybox using normal and reflection vectors
	vec3 v = normalize(V); 
	vec3 r = reflect(-v, N);
	vec4 nits = texture(radiancemap, r); //vec3 for uv, vertex position

	float NdotV = dot(N,v);

	float shadow = ShadowCalculation(positionLightSpace);
	//incorporate Fresnel effect to the environment map + the image
	vec3 shaded =  vec3((1-shadow) * (FresnelReflectance(vec3(reflectance), NdotV ) * nits.xyz) + imageColour.xyz);
	
	FragmentColour = vec4(shaded *exposure, 1.f);	//don't mess with the alphas, that is bad news

	//testing purposes
//	float depth = texture(shadowmap,uv.xy).r;
	//FragmentColour = vec4(vec3(depth),1.f);
	//FragmentColour = texture(shadowmap, uv.xy);
}
