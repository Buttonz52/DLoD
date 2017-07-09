#version 410

// interpolated colour received from vertex stage
in vec3 Colour;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform sampler2D sampler;
uniform int hasTexture;
uniform int mixColour;
uniform float mixAmount;
uniform float transparency;
in vec2 uv;
void main(void)
{
//if texture exists, render that. Otherwise, render colour.
	if (hasTexture==1) {
		//Mix with colour and texture
		vec4 texColour = texture(sampler, uv);

		if (mixColour == 1) {
			FragmentColour = vec4(mix(texColour.xyz, Colour, mixAmount),transparency);
		}

		else {
			FragmentColour = vec4(texColour);
		}

	}
	else {
	//just colour
		FragmentColour = vec4(Colour,transparency);
	}
}
