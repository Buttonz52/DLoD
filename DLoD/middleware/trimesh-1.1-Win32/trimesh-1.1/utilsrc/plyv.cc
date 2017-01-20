/*
Szymon Rusinkiewicz
Stanford Graphics Lab

plyv.cc
A *really* bare-bones .ply file viewer.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "triutil.h"
#include "trimesh.h"
#include <GL/glut.h>


#define LBUTTON 1<<2
#define MBUTTON 1<<1
#define RBUTTON 1<<0
#define ROTBUTTON LBUTTON
#define TRANSXYBUTTON MBUTTON
#define TRANSZBUTTON (LBUTTON | MBUTTON)
#define LIGHTBUTTON RBUTTON
#define WHEEL_UP 3
#define WHEEL_DOWN 4

#define DOF 20.0f
#define FOV float(M_SQRT1_2)
#define DEPTH_FUDGE 0.2f
#define ROT_MULT 2.5f
#define TRANSZ_MULT 4.5f
#define DIFFUSE 0.9f
#define SPECULAR 0.25f
#define AMBIENT 0.05f
#define EDGE_AMBIENT 0.2f
#define EDGE_DIFFUSE (1.0f - EDGE_AMBIENT)
#define LIGHT 0.85f

#ifdef WIN32

# include <limits.h>
# include <windows.h>
# define usleep(x) Sleep((x)/1000)
  // Handling of timestamps and determination of time intervals
  struct timestamp { LARGE_INTEGER t; };
  static inline float LI2f(const LARGE_INTEGER &li)
  {
	// Workaround for MSVC compiler bug.  Sigh.
	float f = unsigned(li.HighPart) >> 16;  f *= 65536.0f;
	f += unsigned(li.HighPart) & 0xffff;    f *= 65536.0f;
	f += unsigned(li.LowPart) >> 16;        f *= 65536.0f;
	f += unsigned(li.LowPart) & 0xffff;
	return f;
  }
  static inline float operator - (const timestamp &t1, const timestamp &t2)
  {
	static LARGE_INTEGER PerformanceFrequency;
	static int status = QueryPerformanceFrequency(&PerformanceFrequency);
	if (status == 0) return 1.0f;
	return (LI2f(t1.t) - LI2f(t2.t)) / LI2f(PerformanceFrequency);
  }
  static inline timestamp now()
  {
	timestamp t;
	QueryPerformanceCounter(&t.t);
	return t;
  }

#else

# include <sys/time.h>
# include <unistd.h>
  typedef struct timeval timestamp;
  static inline float operator - (const timestamp &t1, const timestamp &t2)
  {
	return (float)(t1.tv_sec  - t2.tv_sec) +
	       1.0e-6f*(t1.tv_usec - t2.tv_usec);
  }
  static inline timestamp now()
  {
	timestamp t;
	gettimeofday(&t, NULL);
	return t;
  }

#endif


// Global vars
TriMesh *themesh;
float meshsize;

float pos[3];
float rot, rotaxis[3];
float rot_depth;
float lightdir[3];
bool dospin;
float spinspeed, spinaxis[3];
timestamp last_time;

bool use_dlist = false;
bool draw_edges = false;


// Various trivial utility functions
void cls()
{
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);
	glClearColor(0, 0, 0, 1);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void need_redraw() { glutPostRedisplay(); }
void commit() { glutSwapBuffers(); }
int screenx() { return glutGet(GLUT_WINDOW_WIDTH); }
int screeny() { return glutGet(GLUT_WINDOW_HEIGHT); }

void resetview()
{
	meshsize = 2.0f*themesh->bsphere->r;
	pos[0] = themesh->bsphere->center[0];
	pos[1] = themesh->bsphere->center[1];
	pos[2] = themesh->bsphere->center[2] + 4.0f * themesh->bsphere->r;
	rot = 0;
	rotaxis[0] = rotaxis[1] = 0;  rotaxis[2] = 1;
	rot_depth = 4.0f * themesh->bsphere->r;
	lightdir[0] = lightdir[1] = 0;  lightdir[2] = 1;
	dospin = false;
}


// Actually draw the mesh
void drawmesh()
{
	if (use_dlist) {
		static bool allocated = false;
		if (allocated) {
			glCallList(1);
			return;
		}
		glNewList(1, GL_COMPILE);
		allocated = true;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, themesh->vertices);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, themesh->normals);
	if (themesh->colors) {
#if 0
		static unsigned char *rgbacolors = NULL;
		if (!rgbacolors) {
			rgbacolors = new unsigned char[4*themesh->numvertices];
			for (int i=0; i < themesh->numvertices; i++) {
				rgbacolors[4*i  ] = themesh->colors[i][0];
				rgbacolors[4*i+1] = themesh->colors[i][1];
				rgbacolors[4*i+2] = themesh->colors[i][2];
				rgbacolors[4*i+3] = 255;
			}
		}
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, rgbacolors);
#else
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, themesh->colors);
#endif
	}

	if (draw_edges) {
		glPolygonOffset(1.0f, 1.0f);
		glEnable(GL_POLYGON_OFFSET_FILL);
	}

	const int *t = themesh->tstrips;
	const int *end = themesh->tstrips + themesh->tstripdatalen;
	while (t < end) {
		const int *t1 = t+1;
		while (*t1 != -1)
			t1++;
		glDrawElements(GL_TRIANGLE_STRIP, t1-t, GL_UNSIGNED_INT, t);
		t = t1 + 1;
	}

	if (draw_edges) {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT, GL_LINE);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisable(GL_COLOR_MATERIAL);
		GLfloat global_ambient[] = { EDGE_AMBIENT, EDGE_AMBIENT, EDGE_AMBIENT, 0.0f };
		GLfloat light0_diffuse[] = { EDGE_DIFFUSE, EDGE_DIFFUSE, EDGE_DIFFUSE, 0.0f };
		GLfloat light0_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
		GLfloat mat_diffuse[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
		const int *t = themesh->tstrips;
		const int *end = themesh->tstrips + themesh->tstripdatalen;
		while (t < end) {
			const int *t1 = t+1;
			while (*t1 != -1)
				t1++;
			glDrawElements(GL_TRIANGLE_STRIP, t1-t, GL_UNSIGNED_INT, t);
			t = t1 + 1;
		}
		glPolygonMode(GL_FRONT, GL_FILL);
	}


	if (use_dlist) {
		glEndList();
		glCallList(1);
	}
}


// Redraw function
void redraw()
{
	// Set up camera and lighting
	cls();

	float nearplane = rot_depth / DOF;
	float farplane = rot_depth * DOF;
	float diag = sqrt(float(sqr(screenx()) + sqr(screeny())));
	float top = (float) screeny()/diag * 0.5f*FOV * nearplane;
	float bottom = -top;
	float right = (float) screenx()/diag * 0.5f*FOV * nearplane;
	float left = -right;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, nearplane, farplane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat light0_position[] = { lightdir[0], lightdir[1], lightdir[2], 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glRotatef(180.0f/M_PI*rot, rotaxis[0], rotaxis[1], rotaxis[2]);
	glTranslatef(-pos[0], -pos[1], -pos[2]);

	GLfloat mat_diffuse[4] = { DIFFUSE, DIFFUSE, DIFFUSE, 0 };
	GLfloat mat_specular[4] = { SPECULAR, SPECULAR, SPECULAR, 0 };
	GLfloat mat_shininess[] = { 127 };
	GLfloat global_ambient[] = { AMBIENT, AMBIENT, AMBIENT, 1 };
	GLfloat light0_ambient[] = { 0, 0, 0, 1 };
	GLfloat light0_diffuse[] = { LIGHT, LIGHT, LIGHT, 1.0 };
	GLfloat light0_specular[] = { LIGHT, LIGHT, LIGHT, 1.0 };
	// light0_position is set above...
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	if (themesh->colors) {
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	} else {
		glDisable(GL_COLOR_MATERIAL);
	}

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Draw
	timestamp t1 = now();

	drawmesh();
	commit();

	glFinish();
	timestamp t2 = now();
	printf("\rElapsed time: %.2f sec.", t2 - t1);
	fflush(stdout);
}


// Helper functions for various sorts of mouse events
void rotate(float ox, float oy, float nx, float ny)
{
	// Random multiplicative factor - adjust to taste
	nx += (ROT_MULT-1)*(nx-ox);  
	ny += (ROT_MULT-1)*(ny-oy);

	quat currq;
	RotAndAxis2Q(rot, rotaxis, currq);
	vec viewdir = { 0, 0, -1 };
	currq[0] = -currq[0];
	QRotate(viewdir, currq);
	currq[0] = -currq[0];

	point rotcenter = { pos[0] + rot_depth*viewdir[0],
			    pos[1] + rot_depth*viewdir[1],
			    pos[2] + rot_depth*viewdir[2] };

	quat q;
	Mouse2Q(ox, oy, nx, ny, q);
	QCompose(q, currq, currq);
	Q2RotAndAxis(currq, rot, rotaxis);

	vec newviewdir = { 0, 0, -1 };
	currq[0] = -currq[0];
	QRotate(newviewdir, currq);

	pos[0] = rotcenter[0] - rot_depth*newviewdir[0];
	pos[1] = rotcenter[1] - rot_depth*newviewdir[1];
	pos[2] = rotcenter[2] - rot_depth*newviewdir[2];


	float this_rot;
	Q2RotAndAxis(q, this_rot, spinaxis);

	timestamp new_time = now();
	float dt = new_time - last_time;

	const float time_const = 0.2f;
	if (dt > time_const)
		spinspeed = this_rot / dt;
	else
		// Exponential decay.  Math trick.  Think about it.
		spinspeed = (this_rot/time_const) +
			(1.0f - dt/time_const)*spinspeed;


	last_time = new_time;
	need_redraw();
}

void startspin()
{
	timestamp new_time = now();
	float dt = new_time - last_time;

	if ((dt < 0.2f) && (fabs(spinspeed) > 0.05f))
		dospin = true;
}

void move(float dx, float dy, float dz)
{
	float scalefactor = -0.5f * rot_depth;
	dx *= scalefactor * FOV;
	dy *= scalefactor * FOV;
	dz = rot_depth * (exp(-0.5f * TRANSZ_MULT * dz) - 1.0f);

	vec xaxis = {1, 0, 0};
	vec yaxis = {0, 1, 0};
	vec zaxis = {0, 0, 1};

	quat currq;
	RotAndAxis2Q(rot, rotaxis, currq);

	currq[0] = -currq[0];
	QRotate(xaxis, currq);
	QRotate(yaxis, currq);
	QRotate(zaxis, currq);

	pos[0] += xaxis[0]*dx + yaxis[0]*dy + zaxis[0]*dz;
	pos[1] += xaxis[1]*dx + yaxis[1]*dy + zaxis[1]*dz;
	pos[2] += xaxis[2]*dx + yaxis[2]*dy + zaxis[2]*dz;

	rot_depth += dz;
	need_redraw();
}

void relight(float x, float y)
{
	float theta = M_PI*sqrt(x*x+y*y);
	if (theta > M_PI)
		theta = M_PI;
	float phi = atan2(y, x);

	lightdir[0] = sin(theta)*cos(phi);
	lightdir[1] = sin(theta)*sin(phi);
	lightdir[2] = cos(theta);  

	need_redraw();
}


// Figure out where the surface is, so we know where to put the center of
// rotation.
void update_depth()
{
	float d;
	glReadPixels(screenx()/2, screeny()/2,
		     1, 1,
		     GL_DEPTH_COMPONENT, GL_FLOAT,
		     &d);
	if (d > 0.9999f)
		return;

	double P[16], M[16]; int V[4];
	glGetDoublev(GL_PROJECTION_MATRIX, P);
	glGetDoublev(GL_MODELVIEW_MATRIX, M);
	glGetIntegerv(GL_VIEWPORT, V);
	double x, y, z;
	gluUnProject(screenx()/2, screeny()/2, d,
		     M, P, V,
		     &x, &y, &z);

	vec objpos = { x, y, z };
	float objdist = Dist(pos, objpos);

	float dist2center = Dist(pos, themesh->bsphere->center);
	rot_depth = min(objdist * (1.0f + DEPTH_FUDGE*FOV), dist2center);
}


// Handle mouse movements - called by the mouse callback
void domouse(float mousex, float mousey, unsigned b)
{
	static float lastmousex, lastmousey;
	static unsigned lastb = 0;

	// Handle rotation
	if ((b == ROTBUTTON) && (!lastb))
		spinspeed = 0;
	if ((b == ROTBUTTON) && (lastb == ROTBUTTON))
		rotate(lastmousex, lastmousey, mousex, mousey);
	if ((!b) && (lastb == ROTBUTTON))
		startspin();

	// Handle translation
	if ((b == TRANSXYBUTTON) && (lastb == TRANSXYBUTTON))
		move(mousex - lastmousex, mousey - lastmousey, 0);
	if ((b == TRANSZBUTTON) && (lastb == TRANSZBUTTON))
		move(0, 0, lastmousey - mousey - (lastmousex - mousex));

	// Handle lighting
	if (b == LIGHTBUTTON)
		relight(mousex, mousey);


	if (!b)
		update_depth();

	lastmousex = mousex;
	lastmousey = mousey;
	lastb = b;
}


// The idle loop - handles auto-spin
void idle()
{
	if (!dospin) {
		usleep(10000);
		return;
	}

	timestamp new_time = now();
	float dt = new_time - last_time;

	quat currq;
	RotAndAxis2Q(rot, rotaxis, currq);
	vec viewdir = { 0, 0, -1 };
	currq[0] = -currq[0];
	QRotate(viewdir, currq);
	currq[0] = -currq[0];

	point rotcenter = { pos[0] + rot_depth*viewdir[0],
			    pos[1] + rot_depth*viewdir[1],
			    pos[2] + rot_depth*viewdir[2] };

	quat q;
	RotAndAxis2Q(spinspeed*dt, spinaxis, q);
	QCompose(q, currq, currq);
	Q2RotAndAxis(currq, rot, rotaxis);

	vec newviewdir = { 0, 0, -1 };
	currq[0] = -currq[0];
	QRotate(newviewdir, currq);

	pos[0] = rotcenter[0] - rot_depth*newviewdir[0];
	pos[1] = rotcenter[1] - rot_depth*newviewdir[1];
	pos[2] = rotcenter[2] - rot_depth*newviewdir[2];

	last_time = new_time;
	need_redraw();
}


// Mouse and keyboard GLUT callbacks
static unsigned buttonstate = 0;

void mousemotionfunc(int x, int y)
{
	float r = (float) screenx() / screeny();
	float mousex, mousey;
	if (r > 1.0f) {
		mousex = r * (2.0f*x/screenx() - 1.0f);
		mousey = 1.0f - 2.0f*y/screeny();
	} else {
		mousex = 2.0f*x/screenx() - 1.0f;
		mousey = (1.0f - 2.0f*y/screeny()) / r;
	}

	domouse(mousex, mousey, buttonstate);
}

void mousebuttonfunc(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			buttonstate ^= LBUTTON; break;
		case GLUT_MIDDLE_BUTTON:
			buttonstate ^= MBUTTON; break;
		case GLUT_RIGHT_BUTTON:
			buttonstate ^= RBUTTON; break;
		case WHEEL_UP:
			move(0, 0, -0.05f); break;
		case WHEEL_DOWN:
			move(0, 0, 0.05f); break;
		default:
			return;
	}

	dospin = false;
	mousemotionfunc(x, y);
}

void keyboardfunc(unsigned char key, int x, int y)
{
	switch (key) {
		case 'z':
			move(0, 0, 0.05f); break;
		case 'Z':
			move(0, 0, -0.05f); break;
		case 'e':
			draw_edges = !draw_edges; need_redraw(); break;
		case ' ':
			resetview();
			need_redraw();
			break;
		case '\033': // Esc
		case '\021': // Ctrl-Q
		case 'Q':
		case 'q':
			exit(0);
	}
}


void usage(const char *myname)
{
	fprintf(stderr, "Usage: %s [-dlist] in.ply\n", myname);
	exit(1);
}


int main(int argc, char *argv[])
{
	// Fire up glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInit(&argc, argv);

	// Read in .ply file
	if (argc < 2)
		usage(argv[0]);

	const char *plyfilename = argv[1];
	if (argc > 2 && !strcasecmp(argv[1], "-dlist")) {
		use_dlist = true;
		plyfilename = argv[2];
	}

	themesh = TriMesh::ReadPly(plyfilename);
	if (!themesh) {
		fprintf(stderr, "Couldn't open file %s\n", plyfilename);
		exit(1);
	}

	// Compute all the stuff we'll need...
	themesh->free_confidences();
	themesh->need_normals();
	themesh->need_tstrips();
	themesh->free_adjacentfaces(); // Hack to get some more memory...
	themesh->free_faces();
	themesh->need_bsphere();

	// Create the window, register callbacks, go for it
	glutCreateWindow(argv[0]);

	glutDisplayFunc(redraw);
	glutMouseFunc(mousebuttonfunc);
	glutMotionFunc(mousemotionfunc);
	glutKeyboardFunc(keyboardfunc);
	glutIdleFunc(idle);   

	resetview();

	glutMainLoop();
}
