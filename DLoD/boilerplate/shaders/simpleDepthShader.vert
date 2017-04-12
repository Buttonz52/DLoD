#version 410
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 modelview;
uniform mat4 lightSpaceMatrix;
out vec3 pos;
void main()
{
	gl_Position = lightSpaceMatrix * model *  vec4(position, 1.0);
	pos = position;
}