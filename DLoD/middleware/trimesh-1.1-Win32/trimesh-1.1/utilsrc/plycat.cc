/*
Szymon Rusinkiewicz
Stanford Graphics Lab

plycat.cc
Output vertices and faces from a .ply file as ASCII.
*/

#include <stdio.h>
#include <stdlib.h>
#include "trimesh.h"


void usage(const char *myname)
{
	fprintf(stderr, "Usage: %s in.ply\n", myname);
	exit(1);
}


int main(int argc, char *argv[])
{
	if (argc < 2)
		usage(argv[0]);
	const char *infilename = argv[1];


	// Read in the input .ply file
	TriMesh *in = TriMesh::ReadPly(infilename);
	if (!in) {
		fprintf(stderr, "Couldn't read ply file %s\n", infilename);
		usage(argv[0]);
	}

	// Make sure that we have faces, not tstrips
	in->need_faces();

	// Write out vertices
	printf("\n%d vertices:\n", in->numvertices);
	for (int i=0; i < in->numvertices; i++)
		printf("%f %f %f\n", in->vertices[i][0],
			in->vertices[i][1], in->vertices[i][2]);

	// Write out faces
	printf("\n%d faces:\n", in->numfaces);
	for (int i=0; i < in->numfaces; i++)
		printf("%d %d %d\n", in->faces[i][0],
			in->faces[i][1], in->faces[i][2]);
}
