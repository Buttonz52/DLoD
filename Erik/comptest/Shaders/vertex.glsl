

#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTextureCoords;

uniform mat4 view;
uniform mat4 model;
uniform mat4 proj;

// output to be interpolated between vertices and passed to the fragment stage
out vec2 textureCoords;
out vec4 sun;
out vec4 normal;
out vec4 position;

void main()
{
  // assign vertex position without modification
  position = model * vec4(VertexPosition, 1.0);
  gl_Position = proj * view * position;
  
  normal = model * vec4(VertexPosition,0);

  sun = vec4(0,0,0,1);

  // assign output colour to be interpolated
  textureCoords = VertexTextureCoords;
    
}
