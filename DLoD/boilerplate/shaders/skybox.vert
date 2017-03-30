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

// output to be interpolated between vertices and passed to the fragment stage
uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 lightPosition;

out vec3 uv;
void main()
{
	//very arbitrary scale for the moment; testing purposes.
    // assign vertex position without modification

	vec4 vertexCameraSpace = modelview * model * vec4(VertexPosition,1.0);
	uv = VertexPosition;		//vertex coords instead of UV coords for skybox
    gl_Position = projection * vertexCameraSpace;    
}
