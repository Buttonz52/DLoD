#version 410

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColour;

uniform mat4 MVP;
//uniform aspectRatio;

out vec3 Colour;

void main()
{
   //gl_Position = MVP * vec4(VertexPosition.x/aspectRatio, VertexPosition.yz, 1.0); 
    gl_Position = MVP * vec4(VertexPosition,1.0);
	Colour = VertexColour;
}
