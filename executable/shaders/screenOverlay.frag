#version 410

// interpolated colour received from vertex stage
in vec3 Colour;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
uniform sampler2D sampler;
uniform int hasTexture;
uniform int mixColour;
uniform int isFontTex;
uniform float mixAmount;
uniform float transparency;
in vec2 UV;
void main(void)
{
//if texture exists, render that. Otherwise, render colour.
	if (hasTexture==1) {
		//Mix with colour and texture
		vec4 texColour = texture(sampler, UV);
		if (texColour.w > 0.f) {

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
					texColour = vec4(Colour, transparency);
				}
			}

			if (mixColour == 1) {
				if (texColour.w > 0.f)
					FragmentColour = vec4(mix(texColour.xyz, Colour, mixAmount),transparency);
				else
					discard;
			}

			else {
				FragmentColour = vec4(texColour.xyz,transparency);
			}
		}
		else {
			discard;
		}

	}
	else {
	//just colour
		FragmentColour = vec4(Colour,transparency);
	}
}
