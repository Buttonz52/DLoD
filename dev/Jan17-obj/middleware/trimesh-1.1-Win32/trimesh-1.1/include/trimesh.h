#ifndef TRIMESH_H
#define TRIMESH_H

/*
Szymon Rusinkiewicz
Stanford Graphics Lab

trimesh.h
A triangle mesh, including various properties...

Note that most of this stuff is not allocated when the object is first created,
so if you want to use anything besides vertices and faces, you need to call
need_normals() or whatever...
*/

#include "triutil.h"
#include <vector>
using std::vector;


// Miscellaneous data types
typedef unsigned char color[3];
typedef float conf;
typedef float point[3];
typedef float vec[3];
typedef int face[3];
typedef float quat[4];


// The TriMesh class
class TriMesh {
public:
	// Vertices and per-vertex properties
	int numvertices;
	point *vertices;
	color *colors;
	conf *confidences;
	vec *normals;

	// Faces and tstrips
	int numfaces;
	face *faces;
	int tstripdatalen;
	int *tstrips;

	// Bounding box and sphere
	struct BBox {
		float xmin, xmax, ymin, ymax, zmin, zmax;
		float xlen, ylen, zlen;
	};
	BBox *bbox;

	struct BSphere {
		point center;
		float r;
	};
	BSphere *bsphere;

	// Connectivity information - neighbors and vertex->face mappings
	typedef int *neighborlist;
	neighborlist *neighbors;
	int *numneighbors;
	int minneighbors;
	int maxneighbors;

	typedef int *adjacentfacelist;
	adjacentfacelist *adjacentfaces;
	int *numadjacentfaces;
	int minadjacentfaces;
	int maxadjacentfaces;

	// Edge lengths
	float minedgelength();
	float maxedgelength();
	float rmsedgelength(bool random_sample = true);
	float meanedgelength(bool random_sample = true);
	float medianedgelength(bool random_sample = true);

	// Smooth per-vertex normals
	void SmoothNormals(float smooth);

	// Remove vertices and/or faces
	void RemoveVertices(const vector<bool> &toremove);
	void RemoveUnusedVertices();
	void RemoveFaces(const vector<bool> &toremove);

	// .ply file input and output
	static bool IsPlyFile(const char *plyfile);
	static TriMesh *ReadPly(const char *plyfile);
	void WritePly(const char *plyfile); 

private:
	// Private functions to compute various stuff
	void FindNormals();
	void FindNeighbors();
	void FindAdjacentFaces();
	void FindBBox();
	void FindBSphere();
	void FindTStrips();
	void UnpackTStrips();
	int Tstrip_Next_Tri(int tri, int v1, int v2, bool flip);
	int Tstrip_Next_Vert(int next, int vlast1, int vlast2);
	void Tstrip_Crawl(int v1, int v2, int v3, int next);
	void Tstrip_Bootstrap(int tri);

public:
	// The public interface to the above:  checks whether
	// the needed info already exists and computes it if it doesn't
	void need_faces() { if (!faces && tstrips) UnpackTStrips(); }
	void need_normals() { if (!normals) FindNormals(); }
	void need_neighbors() { if (!neighbors) FindNeighbors(); }
	void need_adjacentfaces() { if (!adjacentfaces) FindAdjacentFaces(); }
	void need_bbox() { if (!bbox) FindBBox(); }
	void need_bsphere() { if (!bsphere) FindBSphere(); }
	void need_tstrips() { if (!tstrips) FindTStrips(); }
	void need_colors() { if (!colors && numvertices) colors = new color[numvertices]; }
	void need_confidences() { if (!confidences && numvertices) confidences = new conf[numvertices]; }

	// The counterparts to the above - free the storage associated with
	// various quantities
	void free_confidences() { if (confidences) { delete [] confidences; confidences = NULL; } }
	void free_colors() { if (colors) { delete [] colors; colors = NULL; } }
	void free_tstrips()
	{
		if (tstrips) {
			delete [] tstrips;
			tstrips = NULL;
		}
		tstripdatalen = 0;
	}
	void free_bsphere() { if (bsphere) { delete bsphere; bsphere = NULL; } }
	void free_bbox() { if (bbox) { delete bbox; bbox = NULL; } }
	void free_adjacentfaces()
	{
		if (adjacentfaces) {
			for (int i=0; i < numvertices; i++)
				delete [] adjacentfaces[i];
			delete [] adjacentfaces;
			delete [] numadjacentfaces;
			adjacentfaces = NULL;
		}
		minadjacentfaces = maxadjacentfaces = 0;
	}
	void free_neighbors()
	{
		if (neighbors) {
			for (int i=0; i < numvertices; i++)
				delete [] neighbors[i];
			delete [] neighbors;
			delete [] numneighbors;
			neighbors = NULL;
		}
		minneighbors = maxneighbors = 0;
	}
	void free_normals() { if (normals) { delete [] normals; normals = NULL; } }
	void free_faces()
	{
		if (faces) {
			delete [] faces;
			faces = NULL;
		}
		numfaces = 0;
	}
	void free_vertices()
	{
		if (vertices) {
			delete [] vertices;
			vertices = NULL;
		}
		numvertices = 0;
	}

	// Constructor and destructor
	TriMesh(int _numvertices = 0, int _numfaces = 0) :
			numvertices(_numvertices), vertices(NULL),
			numfaces(_numfaces), faces(NULL),
			normals(NULL),
			neighbors(NULL), numneighbors(NULL),
			minneighbors(0), maxneighbors(0),
			adjacentfaces(NULL), numadjacentfaces(NULL),
			minadjacentfaces(0), maxadjacentfaces(0),
			bbox(NULL), bsphere(NULL),
			tstrips(NULL), tstripdatalen(0),
			colors(NULL), confidences(NULL)
	{
		if (numvertices)
			vertices = new point[numvertices];
		if (numfaces)
			faces = new face[numfaces];
	}

	~TriMesh()
	{
		free_confidences();
		free_colors();
		free_tstrips();
		free_bsphere();
		free_bbox();
		free_adjacentfaces();
		free_neighbors();
		free_normals();
		free_faces();
		free_vertices();
	}
};

#endif
