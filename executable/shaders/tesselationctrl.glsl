/*
Kiersten Mort - 10109935
Assignment 3


*/// ==========================================================================
// Tesselation control program for cubic for assignment 3
//
// ==========================================================================
#version 410
layout(vertices = 4) out; //tells how big a patch is ->> there are 4 control points in the patch of the Bezier curve
//if putting it here, then has to be a fixed number
//also can set this in the CP code -> glPatchParameteri();
in vec3 Colour[];
out vec3 EvalColour [];
void main() {
//cColor[gl_InvocationID] = vColor[gl_InvocationID];

	EvalColour[gl_InvocationID] = Colour[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; //InvocationID = id of the patch that you are dealing with
	
	//Set the tesselation level.
	gl_TessLevelOuter[0] = 1; //just want one of the curve
	gl_TessLevelOuter[1] = 32; //just set to some number
}