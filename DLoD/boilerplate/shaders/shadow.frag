

#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

in vec3 Colour;
//uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{           
	float depth = texture(shadowMap, TexCoords).x;
	FragColor = vec4(vec3(depth), 1.f);
}

