
#version 410

// Ouput data

void main(){
    // Not really needed, OpenGL does it anyway
	gl_FragDepth = gl_FragCoord.z;
}