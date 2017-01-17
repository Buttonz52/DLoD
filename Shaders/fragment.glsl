

#version 410

uniform sampler2DRect sampler;
uniform int width;
uniform int height;
uniform int lightEnable;

// interpolated colour received from vertex stage
in vec2 textureCoords;
in vec4 sun;
in vec4 position;
in vec4 normal;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void)
{
  
  vec4 n = normalize(lightEnable * normal);
  vec4 l = normalize(sun - position);
  
  float cT = clamp(dot(n,l), 0.2, 1);
  
  vec4 colour = texture(sampler, vec2(textureCoords.x * width, textureCoords.y * height));
  
  if (lightEnable == 1) {
    FragmentColour = colour * cT;
  }
  else {
    FragmentColour = colour;
  }
}
