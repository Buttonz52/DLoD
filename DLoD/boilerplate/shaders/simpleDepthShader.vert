#version 410
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 modelview;
uniform mat4 lightSpaceMatrix;

void main()
{
    FragPosLightSpace = lightSpaceMatrix * modelview * model *  vec4(position, 1.0);
}