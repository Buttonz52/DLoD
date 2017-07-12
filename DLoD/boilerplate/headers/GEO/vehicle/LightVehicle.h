#ifndef LIGHTVEHICLE_H
#define LIGHTVEHICLE_H
#include "Vehicle.h"
class LightVehicle :
	public Vehicle
{
public:
	LightVehicle(Audio *audio, const playerType &type);
	~LightVehicle();
};

#endif
