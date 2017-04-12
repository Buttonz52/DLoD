/*
Kiersten Mort - 10109935
Assignment 3


*/// ==========================================================================
// Tesselation program for cubic Bezier curve for assignment 3
//
// ==========================================================================
#version 410

layout(isolines, equal_spacing) in;	//isolines, equal spacing on the tesselator points on the curve
in vec3 EvalColour [];
out vec3 Colour;
void main() {
	Colour = vec3(normalize(EvalColour[0] + EvalColour[1] + EvalColour[2]).rgb);
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;
	vec4 p3 = gl_in[3].gl_Position;

	float t = gl_TessCoord.x;

	float k0 = (1-t) * (1-t) * (1-t);
	float k1 = 3*t*(1-t)*(1-t);
	float k2 = 3*t*t*(1-t);
	float k3 = t*t*t;

	//math for Bezier curve
	//TODO: find a means to scale it
	gl_Position = (k0*p0 + k1*p1 + k2*p2 + k3*p3);

}
//Tess eval runs once for every point