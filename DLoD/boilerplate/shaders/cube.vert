// ==========================================================================
// Vertex program for barebones GLFW boilerplate
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

// output to be interpolated between vertices and passed to the fragment stage
uniform mat4 modelview;
uniform mat4 projection;
uniform vec3 lightPosition;
out vec3 Colour;
out vec3 N;
out vec3 L;
out vec3 P;
out vec3 V;
out vec3 uv;
void main()
{
	//very arbitrary scale for the moment; testing purposes.
	float scale = 0.05f;
    // assign vertex position without modification
	vec4 vertexCameraSpace = modelview * vec4(VertexPosition*scale,1.0);
	P = vertexCameraSpace.xyz/vertexCameraSpace.w;

	mat3 normalMatrix = mat3(transpose(inverse(modelview)));
    N = normalize(normalMatrix*Normal);
    // assign output colour to be interpolated
    Colour = VertexColour;

	vec4 L4 = modelview * vec4(lightPosition, 1.0);
	L = normalize(L4.xyz - P);
	V = normalize(-P);
    //Pass uv coordinates and position.	
	uv = UV;
    gl_Position = projection * vertexCameraSpace;    
}
