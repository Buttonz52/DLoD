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
uniform float transparency;
uniform float exposure = 1.f;
//uniform samplerCube radiancemap;
//uniform samplerCube irradiancemap;



/*

vec3 fresnelReflectance(vec3 R0, float cos) {
	return R0 + (vec3(1.f)-R0) * pow(1.0-cosine(cos),5.f);
}



*/


void main(void)
{
	//Simple Phong/toon shading
	vec3 R = reflect(-L,N);
	float ka = 0.5;
	float kd = 0.5;
	float ke = 5.0f;
	float ks = 1.0f;
	
	vec3 ambient = ka * Colour;

    // write colour output without modification
	float diffuse = kd * max( 0.0, dot( -N, normalize(L - P)));

    vec3 specular = ks * pow(max(dot(R, V), 0.0), ke) * vec3(1.f);
	
	//vec3 v = normalize(V); 
	//vec3 r = reflect(-v, normal);
	//vec4 nits = texture(radiancemap, r); //vec3 for uv, vertex position
	//vec4 lux = texture(irradiancemap, normal);
	//float NdotV = dot(normal,v);
	//vec3 shaded =  (lux * fresnelReflectance(vec3(0.04), NdotV ) * nits +  vec3(ambient + vec3(diffuse) + specular));
	//FragmentColour = vec4(shaded *exposure, 1.f);	//don't mess with the alphas, that is bad news
    FragmentColour = vec4(ambient + vec3(diffuse) + specular, transparency);
}
