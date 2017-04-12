
#version 410
// Ouput data
out vec4 colour;
in vec3 pos;
void main(){
	gl_FragDepth = 1;
	colour = vec4(pos,1.f);
	//fragdepth = gl_FragCoord.z;
}