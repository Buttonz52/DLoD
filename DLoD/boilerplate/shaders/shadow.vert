#version 410
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

out vec3 Colour;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 modelview;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * modelview * model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = transpose(inverse(mat3(model))) * normal;
    TexCoords = texCoords;
	Colour = colour;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}