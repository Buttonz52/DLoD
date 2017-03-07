#version 410

// interpolated colour received from vertex stage
in vec3 Colour;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform sampler2D sampler;
uniform int hasTexture;
uniform int mixColour;
uniform int isFontTex;
uniform int isRedTransparent;
in vec2 UV;
void main(void)
{
//if texture exists, render that. Otherwise, render colour.
	if (hasTexture==1) {
		//Mix with colour and texture
		vec3 texColour = texture(sampler, UV).xyz;
		if (isRedTransparent ==1) {
			if (texColour.x >0.7 && texColour.y < 0.2 && texColour.z < 0.2) {
				discard;
			}
		}
		//is a font
		if (isFontTex ==1) {
			//make transparent background
			if (texColour.x < 0.5) {
				//we don't want to draw to screen if it is bitmap background 
				discard;
			}
			//else set it to colour of text
			else {
				//FragmentColour = vec4(Colour, 1.f);
				texColour = Colour;
			}
		}

		if (mixColour == 1) {
			FragmentColour = vec4(mix(texColour, Colour, 0.5),1.0);
		}

		else {
			FragmentColour = vec4(texColour,1.f);
		}

	}
	else {
	//just colour
		FragmentColour = vec4(Colour,1);
	}
}
