/*
Szymon Rusinkiewicz
Stanford Graphics Lab

trimesh_tstrips.cc
Computation of triangle strips.
*/

#include <stdio.h>
#include "trimesh.h"
#include "triutil.h"

static bool *done;
static int *next_tstrip_vert;
static int numtstrips;


// Figure out the next triangle we're headed for...
inline int TriMesh::Tstrip_Next_Tri(int tri, int v1, int v2, bool flip)
{
	for (int t=0, n=numadjacentfaces[v1]; t < n; t++) {
		int f = adjacentfaces[v1][t];
		if ((f == tri) || done[f])
			continue;
		for (int i=0; i < 3; i++) {
			int j = (i+(flip?2:1)) % 3;
			if ((faces[f][i] == v1) && (faces[f][j] == v2))
				return f;
		}
	}

	// Couldn't find one...
	return -1;
}

inline int TriMesh::Tstrip_Next_Vert(int next, int vlast1, int vlast2)
{
	for (int i=0; i < 3; i++) {
		int vnext = faces[next][i];
		if ((vnext == vlast1) || (vnext == vlast2))
			continue;
		return vnext;
	}

	// Can't happen
	return -1;
}


// Build a whole strip of triangles, as long as possible...
void TriMesh::Tstrip_Crawl(int v1, int v2, int v3, int next)
{
	// Insert the first tri...
	*next_tstrip_vert++ = v1;
	*next_tstrip_vert++ = v2;
	*next_tstrip_vert++ = v3;

	int vlast2 = v2;
	int vlast1 = v3;

	bool flip = true;
	// Main loop...
	do {
		// Find the next vertex
		int vnext = Tstrip_Next_Vert(next, vlast1, vlast2);

		// Record it
		*next_tstrip_vert++ = vnext;
		done[next] = true;

		flip = !flip;
		vlast2 = vlast1;
		vlast1 = vnext;

		// Try to find the next tri
	} while ((next = Tstrip_Next_Tri(next, vlast2, vlast1, flip)) != -1);
}


// Begin a tstrip, starting with triangle tri
void TriMesh::Tstrip_Bootstrap(int tri)
{
	done[tri] = true;

	// Find two vertices with which to start.
	// We do a bit of lookahead...

	int vert1 = faces[tri][0];
	int vert2 = faces[tri][1];
	int vert3 = faces[tri][2];

	// Try vertices 1 and 2...
	int len12 = 1;
	int nextface12 = Tstrip_Next_Tri(tri, vert1, vert2, true);
	if (nextface12 != -1) {
		len12++;
		if (Tstrip_Next_Tri(nextface12,
				    vert2,
				    Tstrip_Next_Vert(nextface12, vert2, vert1),
				    false) != -1)
			len12++;
	}

	// Try vertices 2 and 3...
	int len23 = 1;
	int nextface23 = Tstrip_Next_Tri(tri, vert2, vert3, true);
	if (nextface23 != -1) {
		len23++;
		if (Tstrip_Next_Tri(nextface23,
				    vert3,
				    Tstrip_Next_Vert(nextface23, vert3, vert2),
				    false) != -1)
			len23++;
	}

	// Try vertices 3 and 1...
	int len31 = 1;
	int nextface31 = Tstrip_Next_Tri(tri, vert3, vert1, true);
	if (nextface31 != -1) {
		len31++;
		if (Tstrip_Next_Tri(nextface31,
				    vert1,
				    Tstrip_Next_Vert(nextface31, vert1, vert3),
				    false) != -1)
			len31++;
	}

	if (len12 >= len23) {
		if (len12 >= len31) {
			if (len12 == 1) {
				// Do a single-triangle-long tstrip.
				*next_tstrip_vert++ = vert1;
				*next_tstrip_vert++ = vert2;
				*next_tstrip_vert++ = vert3;
			} else {
				Tstrip_Crawl(vert3, vert1, vert2, nextface12);
			}
		} else {
			Tstrip_Crawl(vert2, vert3, vert1, nextface31);
		}
	} else {
		if (len23 >= len31)
			Tstrip_Crawl(vert1, vert2, vert3, nextface23);
		else
			Tstrip_Crawl(vert2, vert3, vert1, nextface31);
	}

	*next_tstrip_vert++ = -1;
	numtstrips++;
}


// Turn faces into triangle strips
void TriMesh::FindTStrips()
{
	if (!faces)
		return;
	need_adjacentfaces();

	printf("Building triangle strips... "); fflush(stdout);

	// Allocate more than enough memory
	if (tstrips)
		delete [] tstrips;
	tstrips = new int[4*numfaces];
	next_tstrip_vert = tstrips;
	numtstrips = 0;

	// Allocate array to record what triangles we've already done
	done = new bool[numfaces];
	memset(done, 0, numfaces*sizeof(bool));

	// Build the tstrips
	for (int i=0; i < numfaces; i++) {
		if (!done[i])
			Tstrip_Bootstrap(i);
	}

	delete [] done;

	tstripdatalen = next_tstrip_vert - tstrips;

	printf("%d triangle strips... Done.\n", numtstrips);
}


// Unpack triangle strips into faces
void TriMesh::UnpackTStrips()
{
	if (!tstrips || tstripdatalen < 4)
		return;

	printf("Unpacking triangle strips... "); fflush(stdout);

	// Count number of faces
	numfaces = 0;
	int this_tstrip_len = 0;
	for (int i=0; i < tstripdatalen; i++) {
		if (tstrips[i] == -1) {
			this_tstrip_len = 0;
			continue;
		}
		this_tstrip_len++;
		if (this_tstrip_len >= 3)
			numfaces++;
	}
	printf("%d triangles... ", numfaces); fflush(stdout);

	// Allocate memory
	if (faces)
		delete [] faces;
	faces = new face[numfaces];

	// Write tstrips
	int whichface = 0;
	this_tstrip_len = 0;
	for (int i=0; i < tstripdatalen; i++) {
		if (tstrips[i] == -1) {
			this_tstrip_len = 0;
			continue;
		}
		this_tstrip_len++;
		if (this_tstrip_len < 3)
			continue;
		if (this_tstrip_len % 2) {
			faces[whichface][0] = tstrips[i-2];
			faces[whichface][1] = tstrips[i-1];
		} else {
			faces[whichface][0] = tstrips[i-1];
			faces[whichface][1] = tstrips[i-2];
		}
		faces[whichface++][2] = tstrips[i];
	}

	printf("Done.\n");
}
