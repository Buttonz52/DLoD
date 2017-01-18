#ifndef TRIUTIL_H
#define TRIUTIL_H
/*
Szymon Rusinkiewicz
Stanford Graphics Lab

triutil.h
Various utility functions for dealing with vectors, triangles, and quaternions
*/

#include <math.h>
#include <string.h>
#include <algorithm>
using std::min;
using std::max;
using std::swap;

#define TRACKBALL_R 0.8f

#ifdef WIN32
static double drand48()
{
	return rand() / RAND_MAX;
}
#endif

// Utility functions for square and cube
template <class T>
static inline T sqr(const T &x)
{
	return x*x;
}

template <class T>
static inline T cube(const T &x)
{
	return x*x*x;
}


// Dot product of two 3-vectors
template <class T>
static inline T Dot(const T *x, const T *y)
{
	return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}


// Squared length of a 3-vector
template <class T>
static inline T Len2(const T *x)
{
	return sqr(x[0]) + sqr(x[1]) + sqr(x[2]);
}


// Length of a 3-vector
template <class T>
static inline T Len(const T *x)
{
	return sqrt(Len2(x));
}


// Squared distance between two points in 3-space
template <class T>
static inline T Dist2(const T *x1, const T *x2)
{
	T dx = x2[0] - x1[0];
	T dy = x2[1] - x1[1];
	T dz = x2[2] - x1[2];

	return sqr(dx) + sqr(dy) + sqr(dz);
}


// Distance between two points in 3-space
template <class T>
static inline T Dist(const T *x1, const T *x2)
{
	return sqrt(Dist2(x1,x2));
}


// (a,b,c) cross (d,e,f) -> (g,h,i) */
template <class T>
static inline void CrossProd(const T &a, const T &b, const T &c,
			     const T &d, const T &e, const T &f,
				   T &g,       T &h,       T &i)
{       
	g = b*f-c*e;
	h = c*d-a*f;
	i = a*e-b*d;                    
}
        

// x cross y -> z
template <class T>
static inline void CrossProd(const T *x, const T *y, T *z)
{
	z[0] = x[1]*y[2] - x[2]*y[1];
	z[1] = x[2]*y[0] - x[0]*y[2];
	z[2] = x[0]*y[1] - x[1]*y[0];
}


// Normal of a triangle with vertices p1, p2, p3
// NOTE: not unit-length!
template <class T>
static inline void FindNormal(const T *p1, const T *p2,
			      const T *p3, T *n)
{
	T u1 = p2[0]-p1[0], u2 = p2[1]-p1[1], u3 = p2[2]-p1[2];
	T v1 = p3[0]-p1[0], v2 = p3[1]-p1[1], v3 = p3[2]-p1[2];
                                        
	CrossProd(u1,u2,u3, v1,v2,v3, n[0], n[1], n[2]);
}


// Normalize a 3-vector (make it unit length)
template <class T>
static inline void Normalize(T *v)
{
	T x = Len(v);
	if (x == T(0)) {
		v[0] = v[1] = T(0);
		v[2] = T(1);
		return;
	}

	x = T(1) / x;
	v[0] *= x;
	v[1] *= x;
	v[2] *= x;
}


// Multiply matrices in OpenGL (column-major) order
template <class T>
static inline void MMult(const T *M1,
			 const T *M2,
			 T *Mout)
{
	Mout[ 0] = M1[ 0]*M2[ 0]+M1[ 4]*M2[ 1]+M1[ 8]*M2[ 2]+M1[12]*M2[ 3];
	Mout[ 1] = M1[ 1]*M2[ 0]+M1[ 5]*M2[ 1]+M1[ 9]*M2[ 2]+M1[13]*M2[ 3];
	Mout[ 2] = M1[ 2]*M2[ 0]+M1[ 6]*M2[ 1]+M1[10]*M2[ 2]+M1[14]*M2[ 3];
	Mout[ 3] = M1[ 3]*M2[ 0]+M1[ 7]*M2[ 1]+M1[11]*M2[ 2]+M1[15]*M2[ 3];
	Mout[ 4] = M1[ 0]*M2[ 4]+M1[ 4]*M2[ 5]+M1[ 8]*M2[ 6]+M1[12]*M2[ 7];
	Mout[ 5] = M1[ 1]*M2[ 4]+M1[ 5]*M2[ 5]+M1[ 9]*M2[ 6]+M1[13]*M2[ 7];
	Mout[ 6] = M1[ 2]*M2[ 4]+M1[ 6]*M2[ 5]+M1[10]*M2[ 6]+M1[14]*M2[ 7];
	Mout[ 7] = M1[ 3]*M2[ 4]+M1[ 7]*M2[ 5]+M1[11]*M2[ 6]+M1[15]*M2[ 7];
	Mout[ 8] = M1[ 0]*M2[ 8]+M1[ 4]*M2[ 9]+M1[ 8]*M2[10]+M1[12]*M2[11];
	Mout[ 9] = M1[ 1]*M2[ 8]+M1[ 5]*M2[ 9]+M1[ 9]*M2[10]+M1[13]*M2[11];
	Mout[10] = M1[ 2]*M2[ 8]+M1[ 6]*M2[ 9]+M1[10]*M2[10]+M1[14]*M2[11];
	Mout[11] = M1[ 3]*M2[ 8]+M1[ 7]*M2[ 9]+M1[11]*M2[10]+M1[15]*M2[11];
	Mout[12] = M1[ 0]*M2[12]+M1[ 4]*M2[13]+M1[ 8]*M2[14]+M1[12]*M2[15];
	Mout[13] = M1[ 1]*M2[12]+M1[ 5]*M2[13]+M1[ 9]*M2[14]+M1[13]*M2[15];
	Mout[14] = M1[ 2]*M2[12]+M1[ 6]*M2[13]+M1[10]*M2[14]+M1[14]*M2[15];
	Mout[15] = M1[ 3]*M2[12]+M1[ 7]*M2[13]+M1[11]*M2[14]+M1[15]*M2[15];
}


// Multiply a vector by a matrix
template <class T>
static inline void MVMult(const T *M,
			  const T *V,
			  T *Vout,
			  bool apply_trans = true)
{
	Vout[0] = M[0] * V[0] + M[4] * V[1] + M[8] * V[2];
	Vout[1] = M[1] * V[0] + M[5] * V[1] + M[9] * V[2];
	Vout[2] = M[2] * V[0] + M[6] * V[1] + M[10] * V[2];
	if (apply_trans) {
		Vout[0] += M[12];
		Vout[1] += M[13];
		Vout[2] += M[14];
	}
}


// Basically a replacement for gluProject, with an additional z offset
template <class T>
static inline void Project(const T *P,
			   const T *M,
     			   const T *V,
			   const T *vert,
			   T *coord,
			   T zoffset=0)
{
	T x = vert[0];
	T y = vert[1];
	T z = vert[2];

	T x1 = M[0]*x+M[4]*y+M[8]*z+M[12];
	T y1 = M[1]*x+M[5]*y+M[9]*z+M[13];
	T z1 = M[2]*x+M[6]*y+M[10]*z+M[14];
	T w1 = M[3]*x+M[7]*y+M[11]*z+M[15];

	z1 += zoffset*w1;

	T x2 = P[0]*x1+P[4]*y1+P[8]*z1+P[12]*w1;
	T y2 = P[1]*x1+P[5]*y1+P[9]*z1+P[13]*w1;
	T z2 = P[2]*x1+P[6]*y1+P[10]*z1+P[14]*w1;
	T w2 = P[3]*x1+P[7]*y1+P[11]*z1+P[15]*w1;

	coord[0] = V[0] + V[2] * T(0.5) * (x2/w2 + T(1));
	coord[1] = V[1] + V[3] * T(0.5) * (y2/w2 + T(1));
	coord[2] =               T(0.5) * (z2/w2 + T(1));
}


// Precompute some quantities for FastProject
template <class T>
static inline void FastProjectPrecompute(T *F,
					 const T *P,
					 const T *M,
					 const T *V,
					 T zoffset=0)
{
	T tmp[16];
	memcpy(tmp, M, 16*sizeof(T));

	if (zoffset) {
		tmp[ 2] += zoffset * tmp[ 3];
		tmp[ 6] += zoffset * tmp[ 7];
		tmp[10] += zoffset * tmp[11];
		tmp[14] += zoffset * tmp[15];
	}

	MMult(P, tmp, F);

	F[ 0] = T(0.5) * V[2] * (F[ 0]+F[ 3]) + V[0] * F[ 3];
	F[ 4] = T(0.5) * V[2] * (F[ 4]+F[ 7]) + V[0] * F[ 7];
	F[ 8] = T(0.5) * V[2] * (F[ 8]+F[11]) + V[0] * F[11];
	F[12] = T(0.5) * V[2] * (F[12]+F[15]) + V[0] * F[15];
	F[ 1] = T(0.5) * V[3] * (F[ 1]+F[ 3]) + V[1] * F[ 3];
	F[ 5] = T(0.5) * V[3] * (F[ 5]+F[ 7]) + V[1] * F[ 7];
	F[ 9] = T(0.5) * V[3] * (F[ 9]+F[11]) + V[1] * F[11];
	F[13] = T(0.5) * V[3] * (F[13]+F[15]) + V[1] * F[15];
	F[ 2] = T(0.5)        * (F[ 2]+F[ 3]);
	F[ 6] = T(0.5)        * (F[ 6]+F[ 7]);
	F[10] = T(0.5)        * (F[10]+F[11]);
	F[14] = T(0.5)        * (F[14]+F[15]);
}


// Same as Project (above), but faster.  Uses the "F" precomputed
// by FastProjectPrecompute
template <class T>
static inline void FastProject(const T *F,
			       T x, T y, T z,
			       T &xout, T &yout, T &zout)
{
	T w_rec = T(1)  / (F[ 3]*x + F[ 7]*y + F[11]*z + F[15]);
	xout    = w_rec * (F[ 0]*x + F[ 4]*y + F[ 8]*z + F[12]);
	yout    = w_rec * (F[ 1]*x + F[ 5]*y + F[ 9]*z + F[13]);
	zout    = w_rec * (F[ 2]*x + F[ 6]*y + F[10]*z + F[14]);
}

template <class T>
static inline void FastProject(const T *F,
			       const T *vert,
			       T *coord)
{
	FastProject(F, vert[0], vert[1], vert[2], coord[0], coord[1], coord[2]);
}


// Same as above, but even faster - doesn't bother to compute Z.
// In theory, if you use FastProject but don't use zout, the compiler
// should optimize its computation away.  In practice...
template <class T>
static inline void FastProjectNoZ(const T *F,
				  T x, T y, T z,
				  T &xout, T &yout)
{
	T w_rec = T(1)  / (F[ 3]*x + F[ 7]*y + F[11]*z + F[15]);
	xout    = w_rec * (F[ 0]*x + F[ 4]*y + F[ 8]*z + F[12]);
	yout    = w_rec * (F[ 1]*x + F[ 5]*y + F[ 9]*z + F[13]);
}

template <class T>
static inline void FastProjectNoZ(const T *F,
				  const T *vert,
				  T *coord)
{
	FastProjectNoZ(F, vert[0], vert[1], vert[2], coord[0], coord[1]);
}


// Invert a 4x4 matrix (in OpenGL order) that represents a rigid-body
// (just rotation+translation) transformation
template <class T>
static inline void FastInvert(T *matrix)
{
	swap(matrix[1], matrix[4]);
	swap(matrix[2], matrix[8]);
	swap(matrix[6], matrix[9]);

	T tx = -matrix[12];
	T ty = -matrix[13];
	T tz = -matrix[14];

	matrix[12] = tx*matrix[0] + ty*matrix[4] + tz*matrix[8];
	matrix[13] = tx*matrix[1] + ty*matrix[5] + tz*matrix[9];
	matrix[14] = tx*matrix[2] + ty*matrix[6] + tz*matrix[10];
}


// Find a sphere that encloses the given triangle
// Based on GGems III V.1, by Fernando Lopez-Lopez
// and GGems I "Triangles" by Ronald Goldman
template <class T>
static inline void TriBoundingSphere(const T *p1,
				     const T *p2,
				     const T *p3,
				     T *cent,
				     T &r)
{
	T a[] = { p2[0] - p3[0], p2[1] - p3[1], p2[2] - p3[2] };
	T b[] = { p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2] };
	T c[] = { p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2] };

	T d1 = -Dot(b,c);
	T d2 = -Dot(c,a);
	T d3 = -Dot(a,b);

	// If triangle is obtuse, just want midpt of longest side
	if (d1 <= 0) {

		cent[0] = T(0.5) * (p2[0] + p3[0]);
		cent[1] = T(0.5) * (p2[1] + p3[1]);
		cent[2] = T(0.5) * (p2[2] + p3[2]);
		r = T(0.5) * Len(a);
		return;

	} else if (d2 <= 0) {

		cent[0] = T(0.5) * (p3[0] + p1[0]);
		cent[1] = T(0.5) * (p3[1] + p1[1]);
		cent[2] = T(0.5) * (p3[2] + p1[2]);
		r = T(0.5) * Len(b);
		return;

	} else if (d3 <= 0) {

		cent[0] = T(0.5) * (p1[0] + p2[0]);
		cent[1] = T(0.5) * (p1[1] + p2[1]);
		cent[2] = T(0.5) * (p1[2] + p2[2]);
		r = T(0.5) * Len(c);
		return;

	}

	// Else compute circumcircle
	T e1 = d2*d3;
	T e2 = d3*d1;
	T e3 = d1*d2;
	T e = e1+e2+e3;
	T tmp = T(1) / (T(2) * e);
	cent[0] = ((e2+e3)*p1[0] + (e3+e1)*p2[0] + (e1+e2)*p3[0]) * tmp;
	cent[1] = ((e2+e3)*p1[1] + (e3+e1)*p2[1] + (e1+e2)*p3[1]) * tmp;
	cent[2] = ((e2+e3)*p1[2] + (e3+e1)*p2[2] + (e1+e2)*p3[2]) * tmp;
	r = T(0.5) * sqrt((d1+d2)*(d2+d3)*(d3+d1)/e);
}



// Normalize a quaternion (or any 4-vector)
template <class T>
static inline void QNorm(T *q)
{
	T x = sqrt(sqr(q[0]) + sqr(q[1]) + sqr(q[2]) + sqr(q[3]));
	if (x == T(0)) {
		q[0] = T(1);
		q[1] = q[2] = q[3] = T(0);
		return;
	}

	x = T(1) / x;
	q[0] *= x;
	q[1] *= x;
	q[2] *= x;
	q[3] *= x;
}


// Compose two quaternions
template <class T>
static inline void QCompose(const T *q1, const T *q2, T *q3)
{
	// Separate var to hold output, so you can say QCompose(q1, q2, q2);
	T qout[4];

	qout[0] = q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2]-q1[3]*q2[3];
	qout[1] = q1[0]*q2[1]+q1[1]*q2[0]+q1[2]*q2[3]-q1[3]*q2[2];
	qout[2] = q1[0]*q2[2]+q1[2]*q2[0]+q1[3]*q2[1]-q1[1]*q2[3];
	qout[3] = q1[0]*q2[3]+q1[3]*q2[0]+q1[1]*q2[2]-q1[2]*q2[1];

	QNorm(qout);
	q3[0] = qout[0]; q3[1] = qout[1]; q3[2] = qout[2]; q3[3] = qout[3];
}


// Compute a quaternion from a rotation and axis
template <class T>
static inline void RotAndAxis2Q(T rot, const T *rotaxis, T *q)
{
	T c2 = cos(T(0.5)*rot);
	T s2 = sin(T(0.5)*rot);

	T x = s2 / Len(rotaxis);

	q[0] = c2;
	q[1] = rotaxis[0] * x;
	q[2] = rotaxis[1] * x;
	q[3] = rotaxis[2] * x;
}


// Compute a rotation and axis from a quaternion
template <class T>
static inline void Q2RotAndAxis(const T *q, T &rot, T *rotaxis)
{
	rot = T(2) * acos(q[0]);

	if (rot == T(0)) {
		rotaxis[0] = rotaxis[1] = T(0);
		rotaxis[2] = T(1);
		return;
	}

	rotaxis[0] = q[1];
	rotaxis[1] = q[2];
	rotaxis[2] = q[3];
	Normalize(rotaxis);
}


// Convert an (x,y) normalized mouse position to a position on the trackball
template <class T>
static inline void Mouse2TrackballPos(T x, T y, T *pos)
{
	T r2 = sqr(x) + sqr(y);
	T t = T(0.5) * sqr(TRACKBALL_R);

	pos[0] = x;
	pos[1] = y;
	if (r2 < t)
		pos[2] = sqrt(T(2)*t - r2);
	else
		pos[2] = t / sqrt(r2);
        
	Normalize(pos);
}


// Takes normalized mouse positions (x1, y1) and (x2, y2) and returns a
// quaternion representing a rotation on the trackball
template <class T>
static inline void Mouse2Q(T x1, T y1, T x2, T y2, T *q)
{
	if ((x1 == x2) && (y1 == y2)) {
		q[0] = T(1);
		q[1] = q[2] = q[3] = T(0);
		return;
	}

	T pos1[3], pos2[3];
	Mouse2TrackballPos(x1, y1, pos1);
	Mouse2TrackballPos(x2, y2, pos2);

	T rotaxis[3];
	CrossProd(pos1, pos2, rotaxis);
	Normalize(rotaxis);

	T rotangle = TRACKBALL_R * sqrt(sqr(x2-x1) + sqr(y2-y1));

	RotAndAxis2Q(rotangle, rotaxis, q);
}


// Rotate point x by quaternion q
template <class T>
static inline void QRotate(T *x, const T *q)
{
	T xlen = Len(x);
	if (xlen == T(0))
		return;

	T p[4] = { 0, x[0], x[1], x[2] };
	T qbar[4] = { q[0], -q[1], -q[2], -q[3] };
	T qtmp[4];
	QCompose(q, p, qtmp);
	QCompose(qtmp, qbar, qtmp);

	x[0] = qtmp[1];  x[1] = qtmp[2];  x[2] = qtmp[3];
	Normalize(x);
	x[0] *= xlen;  x[1] *= xlen;  x[2] *= xlen;
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440
#endif

#endif

