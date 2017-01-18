/*
Szymon Rusinkiewicz
Stanford Graphics Lab

trimesh_normals.cc
Computation of per-vertex normals for trimeshes.
*/

#include <stdio.h>
#include "triutil.h"
#include "trimesh.h"


// Finds area-weighted per-vertex normals
void TriMesh::FindNormals()
{
	need_faces();

	printf("Computing normals... "); fflush(stdout);

	if (!normals)
		normals = new vec[numvertices];
	memset(normals, 0, numvertices*sizeof(vec));

	// For each face...
	for (int i=0; i < numfaces; i++) {
		// Find normal
		vec facenormal;
		FindNormal(vertices[faces[i][0]],
			   vertices[faces[i][1]],
			   vertices[faces[i][2]],
			   facenormal);

		// Accumulate. Note that facenormal is not unit-length, so
		// it is really an *area-weighted* normal.
		normals[faces[i][0]][0] += facenormal[0];
		normals[faces[i][0]][1] += facenormal[1];
		normals[faces[i][0]][2] += facenormal[2];

		normals[faces[i][1]][0] += facenormal[0];
		normals[faces[i][1]][1] += facenormal[1];
		normals[faces[i][1]][2] += facenormal[2];

		normals[faces[i][2]][0] += facenormal[0];
		normals[faces[i][2]][1] += facenormal[1];
		normals[faces[i][2]][2] += facenormal[2];
	}

	// Go back and make sure they're all unit-length
	for (int i=0; i < numvertices; i++) {
		float l = Len(normals[i]);
		if (l == 0) {
			normals[i][0] = normals[i][1] = 0;
			normals[i][2] = 1;
			continue;
		}
		float x = 1.0f/l;
		normals[i][0] *= x;
		normals[i][1] *= x;
		normals[i][2] *= x;
	}

	printf("Done.\n");
	SmoothNormals(1);
}


// Smooth out the normals in the mesh.  The parameter tells how much smoothing
// to do.  Less smoothing is done in areas with sharp features.
void TriMesh::SmoothNormals(float smooth)
{
	need_faces();
	need_normals();

	printf("Smoothing normals... "); fflush(stdout);

	vec *newnormals = new vec[numvertices];
	memset(newnormals, 0, numvertices*sizeof(vec));

	for (int i=0; i < numfaces; i++) {
		vec facenormal = {	normals[faces[i][0]][0] +
					normals[faces[i][1]][0] +
					normals[faces[i][2]][0]   ,
					normals[faces[i][0]][1] +
					normals[faces[i][1]][1] +
					normals[faces[i][2]][1]   ,
					normals[faces[i][0]][2] +
					normals[faces[i][1]][2] +
					normals[faces[i][2]][2]	};
		newnormals[faces[i][0]][0] += facenormal[0];
		newnormals[faces[i][1]][0] += facenormal[0];
		newnormals[faces[i][2]][0] += facenormal[0];
		newnormals[faces[i][0]][1] += facenormal[1];
		newnormals[faces[i][1]][1] += facenormal[1];
		newnormals[faces[i][2]][1] += facenormal[1];
		newnormals[faces[i][0]][2] += facenormal[2];
		newnormals[faces[i][1]][2] += facenormal[2];
		newnormals[faces[i][2]][2] += facenormal[2];
	}

	for (int i=0; i < numvertices; i++) {
		Normalize(newnormals[i]);

		// In an effort to avoid smoothing out features, we adjust the
		// smoothing based on how much we're about to change this
		// normal.
		float this_smooth = smooth;
		this_smooth *= Dot(newnormals[i], normals[i]);

		normals[i][0] += this_smooth*newnormals[i][0];
		normals[i][1] += this_smooth*newnormals[i][1];
		normals[i][2] += this_smooth*newnormals[i][2];

		Normalize(normals[i]);
	}

	delete [] newnormals;
	printf("Done.\n");
}

