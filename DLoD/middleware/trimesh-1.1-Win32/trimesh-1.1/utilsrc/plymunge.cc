/*
Szymon Rusinkiewicz
Stanford Graphics Lab

plymunge.cc
Make various simple modifications to ply files.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "trimesh.h"
#include <utility>
#include <vector>
using namespace std;


bool output_color, output_conf, output_tstrips;
const char *infilename, *outfilename;
TriMesh *themesh;

// Check all command-line options for validity, and set infilename, outfilename
bool check_options(int argc, char *argv[])
{
	if (argc < 3)
		return false;

	for (int opt=1; opt < argc-2; opt++) {
		if (!strncasecmp(argv[opt], "-h", 2) ||
		    !strncasecmp(argv[opt], "-?", 2) ||
		    !strncasecmp(argv[opt], "--h", 3))
			return false;
		if (!strncasecmp(argv[opt], "-col", 4) ||
		    !strncasecmp(argv[opt], "-nocol", 6) ||
		    !strncasecmp(argv[opt], "-conf", 5) ||
		    !strncasecmp(argv[opt], "-noconf", 7) ||
		    !strncasecmp(argv[opt], "-tstr", 5) ||
		    !strncasecmp(argv[opt], "-notstr", 7) ||
		    !strncasecmp(argv[opt], "-faceflip", 9) ||
		    !strncasecmp(argv[opt], "-flip", 5))
			continue;
		if (!strncasecmp(argv[opt], "-clip", 5)) {
			if (opt+3 >= argc) {
				fprintf(stderr, "-clip requires an argument.\n");
				return false;
			}
			opt++;
			continue;
		}
		if (!strncasecmp(argv[opt], "-xform", 6)) {
			if (opt+3 >= argc) {
				fprintf(stderr, "-xform requires an argument.\n");
				return false;
			}
			opt++;
			continue;
		}
		if (!strncasecmp(argv[opt], "-scale", 6)) {
			if (opt+5 >= argc) {
				fprintf(stderr, "-scale requires three arguments.\n");
				return false;
			}
			opt += 3;
			continue;
		}
		if (!strncasecmp(argv[opt], "-trans", 6)) {
			if (opt+5 >= argc) {
				fprintf(stderr, "-trans requires three arguments.\n");
				return false;
			}
			opt += 3;
			continue;
		}
		if (!strncasecmp(argv[opt], "-rot", 4)) {
			if (opt+5 >= argc) {
				fprintf(stderr, "-rot requires three arguments.\n");
				return false;
			}
			opt += 3;
			continue;
		}
		if (!strncasecmp(argv[opt], "-q", 2)) {
			if (opt+6 >= argc) {
				fprintf(stderr, "-q requires four arguments.\n");
				return false;
			}
			opt += 4;
			continue;
		}
		fprintf(stderr, "Unrecognized option %s\n", argv[opt]);
		return false;
	}
	infilename = argv[argc - 2];
	outfilename = argv[argc - 1];
	if (infilename[0] == '-' || outfilename[0] == '-') {
		fprintf(stderr, "Missing filename\n");
		return false;
	}
	return true;
}


// Reverse order of vertices within each face
void flip_faces()
{
	printf("Flipping faces... "); fflush(stdout);
	themesh->need_faces();
	themesh->free_tstrips();
	for (int i=0; i < themesh->numfaces; i++)
		swap(themesh->faces[i][0], themesh->faces[i][2]);
	printf("Done.\n");
}


// Clip mesh to the given bounding box
void clip(const char *bboxfilename)
{
	float xmin, xmax, ymin, ymax, zmin, zmax;

	FILE *b = fopen(bboxfilename, "r");
	if (!b) {
		fprintf(stderr, "Couldn't open %s\n", bboxfilename);
		exit(1);
	}


	if (TriMesh::IsPlyFile(bboxfilename)) {

		// Read the header
		char buf[255];
		if (!fgets(buf, 255, b) || !fgets(buf, 255, b)) {
			fprintf(stderr, "Couldn't read %s\n", bboxfilename);
			exit(1);
		}

		if (!strncmp(buf, "format ascii 1.0", 16)) {
			// Assumes bbox.ply is an ascii ply file with
			// exactly two vertices
			do {
				if (!fgets(buf, 255, b)) {
					fprintf(stderr, "Couldn't read %s\n", bboxfilename);
					exit(1);
				}
			} while (strncmp(buf, "end_header", 10));
			float x1, x2, y1, y2, z1, z2;
			if (6 != fscanf(b, "%f%f%f%f%f%f", &x1, &y1, &z1, &x2, &y2, &z2)) {
				fprintf(stderr, "Couldn't read %s\n", bboxfilename);
				exit(1);
			}
			fclose(b);
			xmin = min(x1, x2);  xmax = max(x1, x2);
			ymin = min(y1, y2);  ymax = max(y1, y2);
			zmin = min(z1, z2);  zmax = max(z1, z2);
		} else if (!strncmp(buf, "format binary_big_endian 1.0", 28)) {
			fclose(b);
			TriMesh *m = TriMesh::ReadPly(bboxfilename);
			if (!m) {
				fprintf(stderr, "Couldn't read %s\n", bboxfilename);
				exit(1);
			}
			m->need_bbox();
			xmin = m->bbox->xmin; xmax = m->bbox->xmax;
			ymin = m->bbox->ymin; ymax = m->bbox->ymax;
			zmin = m->bbox->zmin; zmax = m->bbox->zmax;
			delete m;
		} else {
			printf("%s: Unsupported ply file flavor.\n", bboxfilename);
			exit(1);
		}

	} else {
		
		// Not a plyfile - assume it's just 6 numbers
		float x1, x2, y1, y2, z1, z2;
		if (6 != fscanf(b, "%f%f%f%f%f%f", &x1, &y1, &z1, &x2, &y2, &z2)) {
			fprintf(stderr, "Couldn't read %s\n", bboxfilename);
			exit(1);
		}
		fclose(b);
		xmin = min(x1, x2);  xmax = max(x1, x2);
		ymin = min(y1, y2);  ymax = max(y1, y2);
		zmin = min(z1, z2);  zmax = max(z1, z2);

	}


	printf("Clipping... "); fflush(stdout);
	vector<bool> toremove(themesh->numvertices, false);
	for (int i=0; i < themesh->numvertices; i++)
		if (themesh->vertices[i][0] < xmin ||
		    themesh->vertices[i][0] > xmax ||
		    themesh->vertices[i][1] < ymin ||
		    themesh->vertices[i][1] > ymax ||
		    themesh->vertices[i][2] < zmin ||
		    themesh->vertices[i][2] > zmax)
			toremove[i] = true;

	themesh->RemoveVertices(toremove);
}


// Transform mesh by the given .xf file
void xform(const char *xffilename)
{
	FILE *xf = fopen(xffilename, "r");
	if (!xf) {
		fprintf(stderr, "Couldn't read %s\n", xffilename);
		exit(1);
	}

	float xf11, xf12, xf13, xf14;
	float xf21, xf22, xf23, xf24;
	float xf31, xf32, xf33, xf34;
	float xf41, xf42, xf43, xf44;
	if (!fscanf(xf, "%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f",
		    &xf11, &xf12, &xf13, &xf14, &xf21, &xf22, &xf23, &xf24,
		    &xf31, &xf32, &xf33, &xf34, &xf41, &xf42, &xf43, &xf44)) {
		fprintf(stderr, "Couldn't read %s\n", xffilename);
		exit(1);
	}
	fclose(xf);

	printf("XForming... "); fflush(stdout);
	for (int i=0; i < themesh->numvertices; i++) {
		float &x = themesh->vertices[i][0];
		float &y = themesh->vertices[i][1];
		float &z = themesh->vertices[i][2];
		float nx = x*xf11 + y*xf12 + z*xf13 + xf14;
		float ny = x*xf21 + y*xf22 + z*xf23 + xf24;
		float nz = x*xf31 + y*xf32 + z*xf33 + xf34;
		float rw = 1.0f / (x*xf41 + y*xf42 + z*xf43 + xf44);
		x = nx * rw;
		y = ny * rw;
		z = nz * rw;
	}
	printf("Done.\n");
}


// Scale, translate, and rotate mesh
void scale(float sx, float sy, float sz)
{
	printf("Scaling... "); fflush(stdout);
	for (int i=0; i < themesh->numvertices; i++) {
		themesh->vertices[i][0] *= sx;
		themesh->vertices[i][1] *= sy;
		themesh->vertices[i][2] *= sz;
	}
	printf("Done.\n");
}

void trans(float tx, float ty, float tz)
{
	printf("Translating... "); fflush(stdout);
	for (int i=0; i < themesh->numvertices; i++) {
		themesh->vertices[i][0] += tx;
		themesh->vertices[i][1] += ty;
		themesh->vertices[i][2] += tz;
	}
	printf("Done.\n");
}

void rot(float rx, float ry, float rz)
{
	float cx = cos(rx*M_PI/180.0f), sx = sin(rx*M_PI/180.0f);
	float cy = cos(ry*M_PI/180.0f), sy = sin(ry*M_PI/180.0f);
	float cz = cos(rz*M_PI/180.0f), sz = sin(rz*M_PI/180.0f);

	float m11 = cy*cz, m12 = sx*sy*cz-cx*sz, m13 = cx*sy*cz+sx*sz;
	float m21 = cy*sz, m22 = sx*sy*sz+cx*cz, m23 = cx*sy*sz-sx*cz;
	float m31 = -sy,   m32 = sx*cy,          m33 = cx*cy;

	printf("Rotating... "); fflush(stdout);
	for (int i=0; i < themesh->numvertices; i++) {
		float &x = themesh->vertices[i][0];
		float &y = themesh->vertices[i][1];
		float &z = themesh->vertices[i][2];
		float nx = x*m11 + y*m12 + z*m13;
		float ny = x*m21 + y*m22 + z*m23;
		float nz = x*m31 + y*m32 + z*m33;
		x = nx;
		y = ny;
		z = nz;
	}
	printf("Done.\n");
}

void qrot(float qi, float qj, float qk, float qreal)
{
	float rs = 1.0f / (sqr(qi)+sqr(qj)+sqr(qk)+sqr(qreal));
	float m11 = rs * (sqr(qreal)+sqr(qi)-sqr(qj)-sqr(qk));
	float m12 = 2.0f * rs * (qi*qj-qreal*qk);
	float m13 = 2.0f * rs * (qi*qk+qreal*qj);
	float m21 = 2.0f * rs * (qi*qj+qreal*qk);
	float m22 = rs * (sqr(qreal)-sqr(qi)+sqr(qj)-sqr(qk));
	float m23 = 2.0f * rs * (qj*qk-qreal*qi);
	float m31 = 2.0f * rs * (qi*qk-qreal*qj);
	float m32 = 2.0f * rs * (qj*qk+qreal*qi);
	float m33 = rs * (sqr(qreal)-sqr(qi)-sqr(qj)+sqr(qk));
	printf("Rotating... "); fflush(stdout);
	for (int i=0; i < themesh->numvertices; i++) {
		float &x = themesh->vertices[i][0];
		float &y = themesh->vertices[i][1];
		float &z = themesh->vertices[i][2];
		float nx = x*m11 + y*m12 + z*m13;
		float ny = x*m21 + y*m22 + z*m23;
		float nz = x*m31 + y*m32 + z*m33;
		x = nx;
		y = ny;
		z = nz;
	}
	printf("Done.\n");
}


void usage(const char *myname)
{
	fprintf(stderr, "Usage: %s [options...] in.ply out.ply\n", myname);
	fprintf(stderr, "    Options:	-color		Add per-vertex color (white), if not present\n");
	fprintf(stderr, "    		-nocolor	Remove per-vertex color, if present\n");
	fprintf(stderr, "		-conf		Add confidence (0), if not present\n");
	fprintf(stderr, "		-noconf		Remove confidence, if present\n");
	fprintf(stderr, "		-tstrip		Convert faces to tstrips\n");
	fprintf(stderr, "		-notstrip	Convert tstrips to faces\n");
	fprintf(stderr, "		-faceflip	Reverse the order of vertices in each face\n");
	fprintf(stderr, "		-clip bbox.ply	Clip the file to the given bbox\n");
	fprintf(stderr, "		-xform file.xf	Apply the given transformation\n");
	fprintf(stderr, "		-scale sx sy sz	Scale the mesh in X, Y, and Z\n");
	fprintf(stderr, "		-trans tx ty tz	Translate the mesh in X, Y, and Z\n");
	fprintf(stderr, "		-rot rx ry rz	Rotate the mesh (all in degrees)\n");
	fprintf(stderr, "		-q qi qj qk qr	Rotate the mesh by the given quaternion\n");
	exit(1);
}


int main(int argc, char *argv[])
{
	if (!check_options(argc, argv))
		usage(argv[0]);

	// Read in .ply file
	themesh = TriMesh::ReadPly(infilename);
	if (!themesh) {
		fprintf(stderr, "Error reading %s\n", infilename);
		exit(1);
	}

	output_color = !!themesh->colors;
	output_conf = !!themesh->confidences;
	output_tstrips = !!themesh->tstrips;

	// Do requested processing
	for (int opt=1; opt < argc-2; opt++) {
		if (!strncasecmp(argv[opt], "-col", 4)) {
			output_color = true;
		} else if (!strncasecmp(argv[opt], "-nocol", 6)) {
			output_color = false;
		} else if (!strncasecmp(argv[opt], "-conf", 5)) {
			output_conf = true;
		} else if (!strncasecmp(argv[opt], "-noconf", 7)) {
			output_conf = false;
		} else if (!strncasecmp(argv[opt], "-tstr", 5)) {
			output_tstrips = true;
		} else if (!strncasecmp(argv[opt], "-notstr", 7)) {
			output_tstrips = false;
		} else if (!strncasecmp(argv[opt], "-faceflip", 9) ||
			 !strncasecmp(argv[opt], "-flip", 5)) {
			flip_faces();
		} else if (!strncasecmp(argv[opt], "-clip", 5)) {
			clip(argv[opt+1]);
			opt++;
		} else if (!strncasecmp(argv[opt], "-xform", 6)) {
			xform(argv[opt+1]);
			opt++;
		} else if (!strncasecmp(argv[opt], "-scale", 6)) {
			scale(atof(argv[opt+1]), atof(argv[opt+2]), atof(argv[opt+3]));
			opt += 3;
		} else if (!strncasecmp(argv[opt], "-trans", 6)) {
			trans(atof(argv[opt+1]), atof(argv[opt+2]), atof(argv[opt+3]));
			opt += 3;
		} else if (!strncasecmp(argv[opt], "-rot", 4)) {
			rot(atof(argv[opt+1]), atof(argv[opt+2]), atof(argv[opt+3]));
			opt += 3;
		} else if (!strncasecmp(argv[opt], "-q", 2)) {
			qrot(atof(argv[opt+1]), atof(argv[opt+2]), atof(argv[opt+3]), atof(argv[opt+4]));
			opt += 4;
		}
	}

	// Do processing required for selected output options...
	if (output_color && !themesh->colors) {
		themesh->colors = new color[themesh->numvertices];
		memset((void *) themesh->colors,
		       255,
		       themesh->numvertices * sizeof(color));
	} else if (!output_color) {
		themesh->free_colors();
	}

	if (output_conf && !themesh->confidences) {
		themesh->confidences = new conf[themesh->numvertices];
		memset((void *) themesh->confidences,
		       0,
		       themesh->numvertices * sizeof(conf));
	} else if (!output_conf) {
		themesh->free_confidences();
	}

	if (output_tstrips) {
		themesh->need_tstrips();
		themesh->free_faces();
	} else {
		themesh->need_faces();
		themesh->free_tstrips();
	}

	// Write it out
	themesh->WritePly(outfilename);
}


