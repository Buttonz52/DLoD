/*
Szymon Rusinkiewicz
Stanford Graphics Lab

trimesh_remove.cc
Removing sets of vertices or faces from trimeshes.
*/

#include <stdio.h>
#include "triutil.h"
#include "trimesh.h"
#include <vector>
using std::vector;


// Remove the indicated vertices from the TriMesh.
//
// Faces are renumbered to reflect the new numbering of vertices, and any
// faces that included a vertex to be removed will also be removed.
//
// Any per-vertex properties are renumbered along with the vertices.
// Any triangle strips, neighbor lists, or adjacency lists are deleted.
// Note that memory for vertices and properties is not reallocated.
void TriMesh::RemoveVertices(const vector<bool> &toremove)
{
	// Build a table that tells how the vertices will be remapped
	if (!numvertices)
		return;
	printf("Removing vertices... "); fflush(stdout);
	vector<int> remap_table(numvertices);
	int next=0;
	for (int i=0; i < numvertices; i++) {
		if (toremove[i])
			remap_table[i] = -1;
		else
			remap_table[i] = next++;
	}

	// Nothing to delete?
	if (next == numvertices) {
		printf("None removed.\n");
		return;
	}

	// Remap the vertices and per-vertex properties
	for (int i=0; i < numvertices; i++) {
		if (remap_table[i] == -1)  continue;
		vertices[remap_table[i]][0] = vertices[i][0];
		vertices[remap_table[i]][1] = vertices[i][1];
		vertices[remap_table[i]][2] = vertices[i][2];
		if (normals) {
			normals[remap_table[i]][0] = normals[i][0];
			normals[remap_table[i]][1] = normals[i][1];
			normals[remap_table[i]][2] = normals[i][2];
		}
		if (colors) {
			colors[remap_table[i]][0] = colors[i][0];
			colors[remap_table[i]][1] = colors[i][1];
			colors[remap_table[i]][2] = colors[i][2];
		}
		if (confidences)
			confidences[remap_table[i]] = confidences[i];
	}
	printf("%d vertices removed... Done.\n", numvertices - next);
	numvertices = next;

	// Nuke neighbor and adjacency lists
	free_adjacentfaces();
	free_neighbors();

	// Renumber faces
	need_faces();
	free_tstrips();
	int nextface=0;
	for (int i=0; i < numfaces; i++) {
		int n0 = (faces[nextface][0] = remap_table[faces[i][0]]);
		int n1 = (faces[nextface][1] = remap_table[faces[i][1]]);
		int n2 = (faces[nextface][2] = remap_table[faces[i][2]]);
		if ((n0 != -1) && (n1 != -1) && (n2 != -1))
			nextface++;
	}
	numfaces = nextface;
}


// Remove vertices that aren't referenced by any face
void TriMesh::RemoveUnusedVertices()
{
	if (!numvertices)
		return;
	need_faces();
	vector<bool> unused(numvertices, true);
	for (int i=0; i < numfaces; i++)
		unused[faces[i][0]] = unused[faces[i][1]] =
			unused[faces[i][2]] = false;
	RemoveVertices(unused);
}


// Remove faces as indicated by toremove.  Should probably be
// followed by a call to RemoveUnusedVertices()
void TriMesh::RemoveFaces(const vector<bool> &toremove)
{
	need_faces();
	free_tstrips();
	if (!numfaces)
		return;
	printf("Removing faces... "); fflush(stdout);

	int next=0;
	for (int i=0; i < numfaces; i++) {
		if (toremove[i])
			continue;
		faces[next][0] = faces[i][0];
		faces[next][1] = faces[i][1];
		faces[next][2] = faces[i][2];
		next++;
	}
	if (next == numfaces) {
		printf("None removed.\n");
		return;
	}

	printf("%d faces removed... Done.\n", numfaces - next);
	numfaces = next;
}
