#include "..\headers\GameEngine\Text.h"



Text::Text()
{
}


Text::~Text()
{
	mesh.DestroyMesh();
}

void Text::AddShaders(const string &vert, const string &frag) {
	shader.program = shader.InitializeShaders(vert, frag);

}

vec3 Text::lerp(const vec3 &a, const vec3 &b, float t) {
	return (a + (b - a)*t);
}

void Text::segLerp(MySegment *segment, float advance) {
	int segDegree = segment->degree;
	if (segDegree == 1) {
		for (float i = 0.f; i < 1.f; i += 0.01) {
			vec3 l1 = lerp(vec3(segment->x[0], segment->y[0], 0), vec3(segment->x[1], segment->y[1], 0), i);
			mesh.vertices.push_back(vec3(l1.x + advance, l1.y, l1.z));
		}
	}

	else if (segDegree == 3) {
		for (float i = 0.f; i < 1.f; i+=0.01) {
			vec3 l1 = lerp(vec3(segment->x[0], segment->y[0], 0), vec3(segment->x[1], segment->y[1], 0), i);
			vec3 l2 = lerp(vec3(segment->x[1], segment->y[1], 0), vec3(segment->x[2], segment->y[2], 0), i);
			vec3 l3 = lerp(vec3(segment->x[2], segment->y[2], 0), vec3(segment->x[3], segment->y[3], 0), i);

			vec3 l12 = lerp(l1, l2, i);
			vec3 l23 = lerp(l2, l3, i);

			vec3 l123 = lerp(l12, l23, i);
		//	cout << l123.x << " " << l123.y << " " << l123.z << endl;
			mesh.vertices.push_back(vec3(l123.x+advance, l123.y,l123.z));
		}				
	}
}
void Text::InitText(const string &text, const vec3 & colour) {
	vector<MyGlyph> vGlyphs;
	vector<int> degrees;
	float totalAdvance = 0.f;
	GlyphExtractor *ge = new GlyphExtractor();

	//load font file
	if (!ge->LoadFontFile("fonts/Inconsolata.otf")) {
		cout << "Error loading font file" << endl;
	}

	//load glyphs from string
	for (uint i = 0; i < text.size(); i++) {
		MyGlyph newGlyph = ge->ExtractGlyph(text[i]);
		vGlyphs.push_back(newGlyph);
	}

	//get vector of degrees
	for (uint i = 0; i < vGlyphs.size(); i++) {
		for (uint j = 0; j < vGlyphs[i].contours.size(); j++) {
			for (uint k = 0; k < vGlyphs[i].contours[j].size(); k++) {
				degrees.push_back(vGlyphs[i].contours[j][k].degree);
			}
		}
	}

	//get the vertex
	for (uint i = 0; i < vGlyphs.size(); i++) {
		totalAdvance += vGlyphs[i].advance;
		for (uint j = 0; j < vGlyphs[i].contours.size(); j++) {
			for (uint k = 0; k < vGlyphs[i].contours[j].size(); k++) {
				segLerp(&vGlyphs[i].contours[j][k], totalAdvance);
			}
		}
	}

	for (uint i = 0; i < mesh.vertices.size(); i++) {
		mesh.colours.push_back(colour);
	}

	mesh.elementCount = mesh.vertices.size();
	cout << "elem:" <<mesh.elementCount << endl;
	mesh.Initialize();
}

//renders screen.  No model matrix because don't need.
void Text::Render(GLuint type, const vec3 &position, const float _scale)
{
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader.program);
	glBindVertexArray(mesh.vertexArray);

	//Create the model matrix
	mat4 mscale = scale(mat4(), vec3(_scale));
	mat4 translate = glm::translate(mat4(), Normalize(position));//Normalize(position));
	//mat4 rotate = glm::rotate(mat4(), rotateZ, vec3(0, 0, 1));

	//mat4 model = translate*rotate*mscale*mat4();	//translations/rotations
	mat4 model = translate*mscale*mat4();
	vec3 fp = vec3(0, 0, 0);		//focal point
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "MVP"), 1, GL_FALSE, value_ptr(model));
//	glUniform1i(glGetUniformLocation(shader.program, "hasTexture"), hasTexture);
//	glUniform1i(glGetUniformLocation(shader.program, "mixColour"), mixColour);
	glDrawArrays(type, 0, mesh.elementCount);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	//if (hasTexture == 1)
	//	texture.UnbindTexture(GL_TEXTURE_2D);
	// check for an report any OpenGL errors
	CheckGLErrors();
}
//Normalizes a vec3
vec3 Text::Normalize(const vec3 &v) {
	float magn = v.x*v.x + v.y*v.y + v.z*v.z;
	if (magn <= 0) {
		return vec3(0);
	}

	return (v / sqrt(magn));
}