#pragma once
#include "Vehicle.h"
class LargeVehicle :
	public Vehicle
{
public:
	LargeVehicle(Audio * audio, const playerType &type);
	~LargeVehicle();
};

