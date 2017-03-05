#version 410

// interpolated colour received from vertex stage
in vec3 Colour;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform sampler2D sampler;
uniform int hasTexture;
uniform int mixColour;
uniform int isFontTex;
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
			//is a font
			if (isFontTex ==1) {
				vec4 texColour = texture(sampler, UV);

				//make transparent background
				if (texColour.x < 0.5) {
					//we don't want to draw to screen if it is bitmap background 
					discard;
				}
				//else set it to colour of text
				else {
					FragmentColour = vec4(Colour, 1.f);;
				}
			}
			//not a font
			else {
				FragmentColour = texture(sampler, UV);
			}

		}
	}
	else {
	//just colour
		FragmentColour = vec4(Colour,1);
	}
}
