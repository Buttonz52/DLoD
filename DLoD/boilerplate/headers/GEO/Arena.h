#ifndef ARENA_H
#define ARENA_H
#include "GEO.h"
#include "Camera.h"
class Arena :
	public GEO
{
public:
	Arena();
	~Arena();
	vector<GEO> arenaObjects;
	Camera camera;
};
#endif
