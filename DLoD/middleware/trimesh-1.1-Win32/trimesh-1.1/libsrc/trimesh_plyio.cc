/*
Szymon Rusinkiewicz
Stanford Graphics Lab

trimesh_plyio.cc
Code for trimesh I/O to/from .ply files.
This does not read and write generalized .ply files - only a very restricted
subset.  If you want more generalized ply I/O routines, look in
    http://graphics.stanford.edu/software/zippack/
(look for the link to download the original ply libraries)

On the other hand, the routines here are quite a bit more efficient in space
and time for reading the most often-used variants of ply files.
*/

#include <stdio.h>
#include "trimesh.h"
#include "trimesh_endian.h"

#ifdef WIN32
#define strncasecmp strnicmp
#endif


// Is this (possibly) a ply file?
bool TriMesh::IsPlyFile(const char *plyfile)
{
	FILE *f = fopen(plyfile, "rb");
	if (!f)
		return false;
		
	char buf[4];
	fgets(buf, 4, f);
	fclose(f);
	
	return !strncmp(buf, "ply", 3);
}


// Read a triangle mesh from a plyfile.
//
// Note that only a few kinds of ply files are supported.
// The particular conditions are:
// 1. The ply files must be format binary_big_endian 1.0
//                              or binary_little_endian 1.0
// 2. "comment" and "obj_info" lines are allowed, but ignored
// 3. There must be exactly one "element vertex"
// 3a. The vertices must have x, y, and z (as floats)
// 3b. The vertices may contain diffuse_{red,green,blue} (as uchar)
// 3c. The vertices may contain confidence (as float)
// 3d. Other vertex properties will be allowed if they are scalars (char,
//     uchar, int, uint, float).  They will be silently ignored.
// 4. There must be exactly one "element face" or "element tristrips"
// 4a. If there are faces, the only property allowed is
//     property list uchar int vertex_indices, and all faces must be triangles
// 4b. If there are tristrips, the only recognized syntax is
//       element tristrips 1
//       property list int int vertex_indices
//     and the tristrips are separated by an index of -1
// 5. No other elements are allowed.
//
TriMesh *TriMesh::ReadPly(const char *plyfile)
{
	TriMesh *newmesh = NULL;

	bool need_swap;
	int numvertices = 0, numfaces = 0;

	bool have_colors = false, have_conf = false;
	bool have_faces = false, have_tstrips = false;
	int other_prop_len, color_offset, conf_offset;

	char buf[255];
	int result;


	FILE *f = fopen(plyfile, "rb");
	if (!f) {
		perror("fopen");
		fprintf(stderr, "Can't open plyfile [%s]\n", plyfile);
		return NULL;
	}
	printf("Reading %s...\n", plyfile);


	// Read header
	if (!fgets(buf, 255, f) || strncmp(buf, "ply", 3)) {
		fprintf(stderr, "Not a ply file.\n");
		return NULL;
	}

#define GET_LINE() if (!fgets(buf, 255, f)) goto plyreaderror
#define LINE_IS(text) !strncasecmp(buf, text, strlen(text))

	GET_LINE();
	if (LINE_IS("format binary_big_endian 1.0")) {
		need_swap = we_are_little_endian;
	} else if (!LINE_IS("format binary_little_endian 1.0")) {
		need_swap = we_are_big_endian;
	} else {
		fprintf(stderr, "Can only read binary ply files.\n");
		goto plyreaderror;
	}

	GET_LINE();
	while (LINE_IS("obj_info") || LINE_IS("comment"))
		GET_LINE();

	result = sscanf(buf, "element vertex %d\n", &numvertices);
	if (result != 1) {
		fprintf(stderr, "Expected \"element vertex\"\n");
		goto plyreaderror;
	}

	GET_LINE();
	if (!LINE_IS("property float x") && !LINE_IS("property float32 x")) {
		fprintf(stderr, "Expected \"property float x\"\n");
		goto plyreaderror;
	}

	GET_LINE();
	if (!LINE_IS("property float y") && !LINE_IS("property float32 y")) {
		fprintf(stderr, "Expected \"property float y\"\n");
		goto plyreaderror;
	}

	GET_LINE();
	if (!LINE_IS("property float z") && !LINE_IS("property float32 z")) {
		fprintf(stderr, "Expected \"property float z\"\n");
		goto plyreaderror;
	}

	other_prop_len = 0;
	GET_LINE();
	while (LINE_IS("property")) {
		if (LINE_IS("property char") ||
		    LINE_IS("property uchar") ||
		    LINE_IS("property int8") ||
		    LINE_IS("property uint8")) {
			other_prop_len += 1;
		} else if (LINE_IS("property short") ||
			   LINE_IS("property ushort") ||
			   LINE_IS("property int16") ||
			   LINE_IS("property uint16")) {
			other_prop_len += 2;
		} else if (LINE_IS("property int") ||
			   LINE_IS("property uint") ||
			   LINE_IS("property float") ||
			   LINE_IS("property int32") ||
			   LINE_IS("property uint32") ||
			   LINE_IS("property float32")) {
			other_prop_len += 4;
		} else if (LINE_IS("property double") ||
			   LINE_IS("property float64")) {
			other_prop_len += 8;
		} else {
			fprintf(stderr, "Unsupported vertex property: %s\n", buf);
			goto plyreaderror;
		}

		if (LINE_IS("property uchar diffuse_red") ||
		    LINE_IS("property uint8 diffuse_red")) {
			have_colors = true;
			color_offset = other_prop_len - 1;
		} else if (LINE_IS("property float confidence") ||
			   LINE_IS("property float32 confidence")) {
			have_conf = true;
			conf_offset = other_prop_len - 4;
		}

		GET_LINE();
	}


	result = sscanf(buf, "element face %d", &numfaces);
	if (result == 1) {
		have_faces = true;
		GET_LINE();
		if (!LINE_IS("property list uchar int vertex_indices") &&
		    !LINE_IS("property list uint8 int32 vertex_indices"))
			goto plyreaderror;
		GET_LINE();
	} else if (LINE_IS("element tristrips 1")) {
		have_tstrips = true;
		GET_LINE();
		if (!LINE_IS("property list int int vertex_indices") &&
		    !LINE_IS("property list int32 int32 vertex_indices"))
			goto plyreaderror;
		GET_LINE();
	}

	if (!LINE_IS("end_header")) {
		fprintf(stderr, "Expected \"end_header\"\n");
		goto plyreaderror;
	}


	// OK, we think we've parsed the header. Slurp in the actual data...
	newmesh = new TriMesh(numvertices, numfaces);
	if (have_colors)
		newmesh->colors = new color[numvertices];
	if (have_conf)
		newmesh->confidences = new conf[numvertices];

	printf(" Reading %d vertices... ", numvertices); fflush(stdout);
	for (int i=0; i<numvertices; i++) {
		if (!fread((void *)(newmesh->vertices[i]), 12, 1, f))
			goto plyreaderror;
		if (need_swap) {
			swap_float(newmesh->vertices[i][0]);
			swap_float(newmesh->vertices[i][1]);
			swap_float(newmesh->vertices[i][2]);
		}

		if (other_prop_len && !fread((void *)buf, other_prop_len, 1, f))
			goto plyreaderror;

		if (have_colors) {
			memcpy(&newmesh->colors[i][0],
			       buf + color_offset,
			       sizeof(color));
		}
		if (have_conf) {
			memcpy(&newmesh->confidences[i],
			       buf + conf_offset,
			       sizeof(conf));
			if (need_swap)
				swap_float(newmesh->confidences[i]);
		}
	}
	printf("Done.\n");

	if (have_tstrips) {
		printf(" Reading triangle strips... "); fflush(stdout);

		if (!fread((void *)&newmesh->tstripdatalen, 4, 1, f))
			goto plyreaderror;
		if (need_swap)
			swap_4(newmesh->tstripdatalen);

		newmesh->tstrips = new int[newmesh->tstripdatalen];
		if (!fread((void *)newmesh->tstrips, 4*newmesh->tstripdatalen, 1, f))
			goto plyreaderror;
		if (need_swap)
			for (int t=0; t < newmesh->tstripdatalen; t++)
				swap_4(newmesh->tstrips[t]);
	} else if (have_faces) {
		printf(" Reading %d faces... ", numfaces); fflush(stdout);
		for (int i=0; i<numfaces; i++) {
			if (!fread((void *)buf, 1, 1, f))
				goto plyreaderror;
			if (buf[0] != 3) {
				fprintf(stderr, "Non-triangle found in mesh.\n");
			}
			if (!fread((void *)(newmesh->faces[i]), 12, 1, f))
				goto plyreaderror;
			if (need_swap) {
				swap_4(newmesh->faces[i][0]);
				swap_4(newmesh->faces[i][1]);
				swap_4(newmesh->faces[i][2]);
			}
		}
	}
	printf("Done.\n");

	fgets(buf, 2, f);
	if (!feof(f)) {
		fprintf(stderr, "Warning: ignoring excess garbage at end of ply file.\n");
	}

	fclose(f);
	return newmesh;

plyreaderror:
	fclose(f);
	if (newmesh)
		delete newmesh;
	fprintf(stderr, "Error reading plyfile.\n");
	return NULL;
}


// Write mesh to a ply file.  Writes vertices, tstrips (if present), faces (if
// no tstrips), colors, and confidences
void TriMesh::WritePly(const char *plyfile)
{
	bool need_swap = we_are_little_endian; // Always write big-endian
	if (!vertices) {
		fprintf(stderr, "Empty mesh - nothing to write!\n");
		return;
	}
	bool writetstrips = !!tstrips;
	bool writetriangles = (!writetstrips && !!vertices);

	FILE *f = fopen(plyfile, "wb");

	if (!f) {
		fprintf(stderr, "Error: Can't open %s for writing.\n", plyfile);
		return;
	}

	printf("Writing %s... ", plyfile); fflush(stdout);

	// Write header
	fprintf(f, "ply\n"
		   "format binary_big_endian 1.0\n");

	fprintf(f, "element vertex %d\n", numvertices);
	fprintf(f, "property float x\n"
		   "property float y\n"
		   "property float z\n");
	if (colors) {
		fprintf(f, "property uchar diffuse_red\n"
			   "property uchar diffuse_green\n"
			   "property uchar diffuse_blue\n");
	}
	if (confidences) {
		fprintf(f, "property float confidence\n");
	}

	if (writetstrips) {
		fprintf(f, "element tristrips 1\n");
		fprintf(f, "property list int int vertex_indices\n");
	} else if (writetriangles) {
		fprintf(f, "element face %d\n", numfaces);
		fprintf(f, "property list uchar int vertex_indices\n");
	}
	fprintf(f, "end_header\n");
	fflush(f);

	// Write vertices
	for (int i=0; i<numvertices; i++) {
		if (need_swap) {
			float vv[3] = {	vertices[i][0],
					vertices[i][1],
					vertices[i][2] };
			swap_float(vv[0]);
			swap_float(vv[1]);
			swap_float(vv[2]);
			fwrite((void *)(vv), (sizeof(float)), 3, f);
		} else {
			fwrite((void *)(vertices[i]), sizeof(point), 1, f);
		}

		if (colors)
			fwrite((void *)(colors[i]), sizeof(color), 1, f);
		if (confidences) {
			if (need_swap) {
				float c = confidences[i];
				swap_float(c);
				fwrite((void *)&c, sizeof(float), 1, f);
			} else {
				fwrite((void *)&confidences[i], sizeof(float), 1, f);
			}
		}
	}

	if (writetstrips) {
		if (need_swap) {
			int tmp = tstripdatalen;
			swap_4(tmp);
			fwrite((void *)&tmp, sizeof(int), 1, f);
			for (int i=0; i < tstripdatalen; i++) {
				tmp = tstrips[i];
				swap_4(tmp);
				fwrite((void *)&tmp, sizeof(int), 1, f);
			}
		} else {
			fwrite((void *)&tstripdatalen, sizeof(int), 1, f);
			fwrite((void *)tstrips, 4*tstripdatalen, 1, f);
		}
	} else if (writetriangles) {
		char vertsperface = { 3 };
		for (int i=0; i < numfaces; i++) {
			fwrite((void *)&vertsperface, 1, 1, f);
			if (need_swap) {
				int ff[3] = { faces[i][0], faces[i][1], faces[i][2] };
				swap_4(ff[0]);
				swap_4(ff[1]);
				swap_4(ff[2]);
				fwrite((void *)ff, sizeof(face), 1, f);
			} else {
				fwrite((void *)(faces[i]), sizeof(face), 1, f);
			}
		}
	}

	fclose(f);
	printf("Done.\n");
}
