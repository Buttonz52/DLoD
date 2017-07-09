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
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 VertexColour;

// output to be interpolated between vertices and passed to the fragment stage
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 Colour;
out vec2 uv;
void main()
{
	//very arbitrary scale for the moment; testing purposes.
    // assign vertex position without modification

	vec4 vertexCameraSpace = view * model * vec4(VertexPosition,1.0);

    // assign output colour to be interpolated
    Colour = VertexColour;

    //Pass uv coordinates and position.	
	uv = UV;
    gl_Position = projection * vertexCameraSpace;    
}
