#pragma once
#include "Vehicle.h"
class MediumVehicle :
	public Vehicle
{
public:
	MediumVehicle(Audio *audio, const playerType &type);
	~MediumVehicle();
};

