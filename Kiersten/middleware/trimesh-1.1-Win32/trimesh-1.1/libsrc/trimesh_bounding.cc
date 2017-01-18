/*
Szymon Rusinkiewicz
Stanford Graphics Lab

trimesh_bounding.cc
Code for bounding box and bounding sphere.
*/

#include <stdio.h>
#include <float.h>
#include "triutil.h"
#include "trimesh.h"

#define BIGNUM FLT_MAX


void TriMesh::FindBBox()
{
	printf("Computing bounding box... "); fflush(stdout);

	if (bbox)
		delete bbox;
	bbox = new BBox;
	bbox->xmin = bbox->ymin = bbox->zmin = BIGNUM;
	bbox->xmax = bbox->ymax = bbox->zmax = -BIGNUM;

	for (int i=0; i < numvertices; i++) {
		bbox->xmin = min(bbox->xmin, vertices[i][0]);
		bbox->xmax = max(bbox->xmax, vertices[i][0]);
		bbox->ymin = min(bbox->ymin, vertices[i][1]);
		bbox->ymax = max(bbox->ymax, vertices[i][1]);
		bbox->zmin = min(bbox->zmin, vertices[i][2]);
		bbox->zmax = max(bbox->zmax, vertices[i][2]);
	}

	bbox->xlen = bbox->xmax - bbox->xmin;
	bbox->ylen = bbox->ymax - bbox->ymin;
	bbox->zlen = bbox->zmax - bbox->zmin;

	printf("Done.\n"); fflush(stdout);
}


// Based on the bounding sphere code by Jack Ritter in _Graphics Gems_
void TriMesh::FindBSphere()
{
	if (bsphere)
		delete bsphere;
	bsphere = new BSphere;

	printf("Computing bounding sphere... "); fflush(stdout);

	// Pass I - find minima/maxima in each coord
	float xx=BIGNUM, xy, xz, yx, yy=BIGNUM, yz, zx, zy, zz=BIGNUM;
	float XX=-BIGNUM, XY, XZ, YX, YY=-BIGNUM, YZ, ZX, ZY, ZZ=-BIGNUM;

	for (int i = 0; i < numvertices; i++) {
		float x = vertices[i][0];
		float y = vertices[i][1];
		float z = vertices[i][2];
		if (x < xx) { xx = x; xy = y; xz = z; }
		if (x > XX) { XX = x; XY = y; XZ = z; }
		if (y < yy) { yx = x; yy = y; yz = z; }
		if (y > YY) { YX = x; YY = y; YZ = z; }
		if (z < zz) { zx = x; zy = y; zz = z; }
		if (z > ZZ) { ZX = x; ZY = y; ZZ = z; }
	}

	// Set [xyz]span to square of dist btw [xyz]{min,max}
	float dx, dy, dz;

	dx = XX-xx; dy = XY-xy; dz = XZ-xz;
	float xspan = dx*dx+dy*dy+dz*dz;

	dx = YX-yx; dy = YY-yy; dz = YZ-yz;
	float yspan = dx*dx+dy*dy+dz*dz;

	dx = ZX-zx; dy = ZY-zy; dz = ZZ-zz;
	float zspan = dx*dx+dy*dy+dz*dz;

	// Set dia[12] to maximally separated pair
	float dia1x = xx, dia1y = xy, dia1z = xz;
	float dia2x = XX, dia2y = XY, dia2z = XZ;
	float maxspan = xspan;
	if (yspan > maxspan) {
		maxspan = yspan;
		dia1x = yx, dia1y = yy, dia1z = yz;
		dia2x = YX, dia2y = YY, dia2z = YZ;
	}
	if (zspan > maxspan) {
		maxspan = zspan;
		dia1x = zx, dia1y = zy, dia1z = zz;
		dia2x = ZX, dia2y = ZY, dia2z = ZZ;
	}

	// Find initial center
	float cx = (dia1x+dia2x)*0.5f;
	float cy = (dia1y+dia2y)*0.5f;
	float cz = (dia1z+dia2z)*0.5f;

	// Find initial radius, r^2
	dx = dia2x-cx;
	dy = dia2y-cy;
	dz = dia2z-cz;
	float r2 = dx*dx+dy*dy+dz*dz;
	float r = sqrt(r2);

	// Pass II - increment sphere until everything fits
	for (int i = 0; i < numvertices; i++) {
		dx = vertices[i][0] - cx;
		dy = vertices[i][1] - cy;
		dz = vertices[i][2] - cz;
		float current_r2 = dx*dx+dy*dy+dz*dz;
		if (current_r2 > r2) {
			float current_r = sqrt(current_r2);
			r = (r+current_r)*0.5f;
			r2 = r*r;
			float dr = current_r - r;
			float r_current_r = 1.0f / current_r;
			cx = (r*cx + dr*vertices[i][0]) * r_current_r;
			cy = (r*cy + dr*vertices[i][1]) * r_current_r;
			cz = (r*cz + dr*vertices[i][2]) * r_current_r;
		}
	}

	bsphere->center[0] = cx;
	bsphere->center[1] = cy;
	bsphere->center[2] = cz;
	bsphere->r = r;

	// For certain pathological inputs, the above algorithm produces a
	// ReallyBad (tm) result.  As a sanity check, make sure that the
	// bsphere of the bbox isn't smaller...  -- SMR

	float xmin = BIGNUM, ymin = BIGNUM, zmin = BIGNUM;
	float xmax = -BIGNUM, ymax = -BIGNUM, zmax = -BIGNUM;
	for (int i=0; i < numvertices; i++) {
		xmin = min(xmin, vertices[i][0]);
		xmax = max(xmax, vertices[i][0]);
		ymin = min(ymin, vertices[i][1]);
		ymax = max(ymax, vertices[i][1]);
		zmin = min(zmin, vertices[i][2]);
		zmax = max(zmax, vertices[i][2]);
	}

	r = 0.5f * sqrt(sqr(xmax-xmin) + sqr(ymax-ymin) + sqr(zmax-zmin));
	if (r < bsphere->r) {
		bsphere->r = r;
		bsphere->center[0] = 0.5f * (xmin+xmax);
		bsphere->center[1] = 0.5f * (ymin+ymax);
		bsphere->center[2] = 0.5f * (zmin+zmax);
	}

	printf("Done.\n"); fflush(stdout);
}





