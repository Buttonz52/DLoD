// ==========================================================================
// Vertex program cube maps
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColour;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 UV;
layout(location = 4) in vec3 tangent;

// output to be interpolated between vertices and passed to the fragment stage
uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 lightPosition;

out vec3 Colour;
out vec3 N;
out vec3 L;
out vec3 P;
out vec3 V;
out vec3 uv;
//out vec3 lVec;
void main()
{
	//very arbitrary scale for the moment; testing purposes.
    // assign vertex position without modification
	vec4 vertexCameraSpace = modelview * model * vec4(VertexPosition,1.0);
	P = vertexCameraSpace.xyz/vertexCameraSpace.w;

	mat3 normalMatrix = mat3(transpose(inverse(modelview)));
    N = normalize(normalMatrix*Normal);
	/*
	    vec3 T = normalize(mat3(transpose(inverse(modelview)))*tangent);
	T = normalize(T-dot(T,N)*N);
	
	vec3 B = normalize(cross(T,N));
	mat3 TBN = mat3(T,B,N);
	*/
    // assign output colour to be interpolated
    Colour = VertexColour;

	vec4 L4 = modelview * vec4(lightPosition, 1.0);
	L = normalize(L4.xyz - P);
	V = normalize(-P);
    //Pass uv coordinates and position.	
	
	//lVec = normalize(vec3(dot (L,T),dot (L,B),dot (L,-N)));

	//uv = UV;
	uv = VertexPosition;		//vertex coords instead of UV coords for skybox
    gl_Position = projection * vertexCameraSpace;    
}
