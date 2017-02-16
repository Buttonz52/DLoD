#version 410

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColour;
layout(location = 2) in vec2 textureCoords;

uniform mat4 model;
out vec3 Colour;
out vec2 UV;
void main()
{
    gl_Position = model * vec4(VertexPosition.xy,0, 1.0);		//just outputs to screen, no model matrix
	Colour = VertexColour;
	UV = textureCoords;
}
