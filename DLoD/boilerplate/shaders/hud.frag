#version 410

// interpolated colour received from vertex stage
in vec3 Colour;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void)
{
	if (Colour.x <0.6 && Colour.y < 0.6 && Colour.z < 0.6) {
		FragmentColour = vec4(vec3(0), 0);
	}
    FragmentColour = vec4(Colour,1);
}
