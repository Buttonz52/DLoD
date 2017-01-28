#ifndef LIGHT_H
#define LIGHT_H
#include "GEO.h"
class Light :
	public GEO
{
public:
	Light();
	~Light();
	float intensity;
};
#endif
