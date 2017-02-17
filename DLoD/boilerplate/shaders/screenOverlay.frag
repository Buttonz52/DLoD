#version 410

// interpolated colour received from vertex stage
in vec3 Colour;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform sampler2D sampler;
uniform int hasTexture;
uniform int mixColour;
in vec2 UV;
void main(void)
{
//if texture exists, render that. Otherwise, render colour.
	if (hasTexture==1) {
		//Mix with colour and texture
		if (mixColour == 1) {
			FragmentColour = vec4(mix(texture(sampler,UV).xyz, Colour, 0.5),1.0);
		}
		else {
		//just texture
			FragmentColour = texture(sampler, UV);
		}
	}
	else {
	//just colour
		FragmentColour = vec4(Colour,1);
	}
}
