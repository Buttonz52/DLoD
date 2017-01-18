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

// output to be interpolated between vertices and passed to the fragment stage
uniform mat4 modelview;
uniform mat4 projection;
out vec3 Colour;
out vec3 N;

void main()
{
	//very arbitrary scale for the moment; testing purposes.
	float scale = 0.01f;
    // assign vertex position without modification
	gl_Position = projection * modelview * vec4(VertexPosition*scale,1.0);

	mat3 normalMatrix = mat3(transpose(inverse(modelview)));
    N = normalize(normalMatrix*Normal);
    // assign output colour to be interpolated
    Colour = VertexColour;

}
