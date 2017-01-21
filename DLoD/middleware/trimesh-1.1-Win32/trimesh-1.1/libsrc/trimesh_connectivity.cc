/*
Szymon Rusinkiewicz
Stanford Graphics Lab

trimesh.cc
Computation of connectivity information (e.g. neighbors) for trimeshes.
*/

#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "triutil.h"
#include "trimesh.h"
#include <algorithm>
using std::nth_element;


#define BIGNUM FLT_MAX
#define BIGINT INT_MAX


// Compute a list of the neighboring vertices of each vertex
void TriMesh::FindNeighbors()
{
	need_faces();

	printf("Computing lists of neighbors... "); fflush(stdout);

	// Step I - compute a first stab at numneighbors, so we know
	// how much mem to allocate.
	// Overestimates by a factor of 2...
	if (!numneighbors)
		numneighbors = new int[numvertices];
	memset(numneighbors, 0, numvertices*sizeof(int));

	for (int i=0; i < numfaces; i++) {
		numneighbors[faces[i][0]] += 2;
		numneighbors[faces[i][1]] += 2;
		numneighbors[faces[i][2]] += 2;
	}

	// OK, Step II - compute the actual neighbors...
	if (neighbors) {
		for (int i=0; i < numvertices; i++)
			delete [] neighbors[i];
		delete [] neighbors;
	}

	neighbors = new neighborlist[numvertices];
	for (int i=0; i < numvertices; i++) {
		neighbors[i] = new int[numneighbors[i]];
		for (int j=0; j < numneighbors[i]; j++)
			neighbors[i][j] = numvertices;
	}

	memset(numneighbors, 0, numvertices*sizeof(int));
	
	for (int i=0; i < numfaces; i++) {
		for (int j=0; j < 3; j++) {
			int me, n1, n2, *p;
			me = faces[i][j];
			p = neighbors[me];
			n1 = faces[i][(j+1)%3];
			n2 = faces[i][(j+2)%3];
			while ((*p != numvertices) && (*p != n1))
				p++;
			if (*p != n1) {
				*p = n1;
				numneighbors[me]++;
			}
			p = neighbors[me];
			while ((*p != numvertices) && (*p != n2))
				p++;
			if (*p != n2) {
				*p = n2;
				numneighbors[me]++;
			}
		}
	}

	// Compute {min|max}neighbors...
	maxneighbors = 0;
	minneighbors = BIGINT;
	for (int i=0; i < numvertices; i++) {
		minneighbors = min(minneighbors, numneighbors[i]);
		maxneighbors = max(maxneighbors, numneighbors[i]);
	}

	printf("Done.\n");
}


// For each vertex, figure out which faces contain that vertex
void TriMesh::FindAdjacentFaces()
{
	need_faces();

	printf("Computing vertex-to-triangle mappings... "); fflush(stdout);

	// Step I - compute numadjacentfaces
	if (!numadjacentfaces)
		numadjacentfaces = new int[numvertices];
	memset(numadjacentfaces, 0, numvertices*sizeof(int));

	for (int i=0; i < numfaces; i++) {
		numadjacentfaces[faces[i][0]]++;
		numadjacentfaces[faces[i][1]]++;
		numadjacentfaces[faces[i][2]]++;
	}

	// Step II - compute the actual vertex->tri lists...
	if (adjacentfaces) {
		for (int i=0; i < numvertices; i++)
			delete [] adjacentfaces[i];
		delete [] adjacentfaces;
	}

	maxadjacentfaces = 0;
	minadjacentfaces = BIGINT;

	adjacentfaces = new adjacentfacelist[numvertices];
	for (int i=0; i < numvertices; i++) {
		adjacentfaces[i] = new int[numadjacentfaces[i]];
		for (int j=0; j<numadjacentfaces[i]; j++)
			adjacentfaces[i][j] = numfaces;
		minadjacentfaces = min(minadjacentfaces, numadjacentfaces[i]);
		maxadjacentfaces = max(maxadjacentfaces, numadjacentfaces[i]);
	}
	
	for (int i=0; i < numfaces; i++) {
		int *p = adjacentfaces[faces[i][0]];
		while (*p != numfaces)
			p++;
		*p = i;
		p = adjacentfaces[faces[i][1]];
		while (*p != numfaces)
			p++;
		*p = i;
		p = adjacentfaces[faces[i][2]];
		while (*p != numfaces)
			p++;
		*p = i;
	}

	printf("Done.\n");
}


// Find the minimum edge length in the mesh
float TriMesh::minedgelength()
{
	need_faces();
	if (!numfaces)
		return 0;

	float min_len = BIGNUM;
	for (int i=0; i < numfaces; i++) {
		min_len = min(min_len, Dist(vertices[faces[i][0]],
					    vertices[faces[i][1]]));
		min_len = min(min_len, Dist(vertices[faces[i][1]],
					    vertices[faces[i][2]]));
		min_len = min(min_len, Dist(vertices[faces[i][2]],
					    vertices[faces[i][0]]));
	}

	return min_len;
}


// Find the maximum edge length in the mesh
float TriMesh::maxedgelength()
{
	need_faces();
	if (!numfaces)
		return 0;

	float max_len = -BIGNUM;
	for (int i=0; i < numfaces; i++) {
		max_len = max(max_len, Dist(vertices[faces[i][0]],
					    vertices[faces[i][1]]));
		max_len = max(max_len, Dist(vertices[faces[i][1]],
					    vertices[faces[i][2]]));
		max_len = max(max_len, Dist(vertices[faces[i][2]],
					    vertices[faces[i][0]]));
	}

	return max_len;
}


// Find the RMS edge length in the mesh.  If random_sample is true,
// uses at most 10000 edges (randomly sampled)
float TriMesh::rmsedgelength(bool random_sample /* = true */)
{
	need_faces();
	if (!numfaces)
		return 0;

	if (numfaces <= 3333)
		random_sample = false;
	int n = random_sample ? 3333 : numfaces;

	float rms_len = 0;
	for (int i=0; i < n; i++) {
		int which = random_sample ? int(numfaces * drand48()) : i;
		rms_len += Dist2(vertices[faces[which][0]],
				 vertices[faces[which][1]]);
		rms_len += Dist2(vertices[faces[which][1]],
				 vertices[faces[which][2]]);
		rms_len += Dist2(vertices[faces[which][2]],
				 vertices[faces[which][0]]);
	}

	return sqrt(rms_len / (3 * n));
}


// Find the mean edge length in the mesh.  If random_sample is true,
// uses at most 10000 edges (randomly sampled)
float TriMesh::meanedgelength(bool random_sample /* = true */)
{
	need_faces();
	if (!numfaces)
		return 0;

	if (numfaces <= 3333)
		random_sample = false;
	int n = random_sample ? 3333 : numfaces;

	float mean_len = 0;
	for (int i=0; i < n; i++) {
		int which = random_sample ? int(numfaces * drand48()) : i;
		mean_len += Dist(vertices[faces[which][0]],
				 vertices[faces[which][1]]);
		mean_len += Dist(vertices[faces[which][1]],
				 vertices[faces[which][2]]);
		mean_len += Dist(vertices[faces[which][2]],
				 vertices[faces[which][0]]);
	}

	return mean_len / (3 * n);
}


// Find the median edge length in the mesh.  If random_sample is true,
// uses at most 10000 edges (randomly sampled)
float TriMesh::medianedgelength(bool random_sample /* = true */)
{
	need_faces();
	if (!numfaces)
		return 0;

	if (numfaces <= 3333)
		random_sample = false;
	int n = random_sample ? 3333 : numfaces;

	vector<float> lengths;
	lengths.reserve(3*n);

	for (int i=0; i < n; i++) {
		int which = (numfaces > 3333) ? int(numfaces * drand48()) : i;
		lengths.push_back(Dist(vertices[faces[which][0]],
				       vertices[faces[which][1]]));
		lengths.push_back(Dist(vertices[faces[which][1]],
				       vertices[faces[which][2]]));
		lengths.push_back(Dist(vertices[faces[which][2]],
				       vertices[faces[which][0]]));
	}

	nth_element(lengths.begin(), &lengths[3*n/2], lengths.end());

	return lengths[n/2];
}



