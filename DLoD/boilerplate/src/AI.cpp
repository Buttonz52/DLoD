#include "GEO\player\AI.h"



AI::AI()
{
}


AI::~AI()
{
}

int AI::DetermineBehaviour()
{
	return 0;
}

void AI::getInput()
{

}

void AI::driveTo(vec3 destination)
{
  // get the position of the vehicle
  mat4 M = vehicle->getModelMatrix();
  vec3 pos = vec3(M[3]);

  vec3 displacement = destination - pos;

  // Get the rotation of the object
  physx::PxVec3 axis = physx::PxVec3(0, 1, 0);
  physx::PxReal angle = 0;
  vehicle->physXVehicle->getRigidDynamicActor()->getGlobalPose().q.toRadiansAndUnitAxis(angle, axis);

  int fix = (axis.y < 0) ? 1 : -1;
  angle *= fix;
  if (angle < 0)
    angle += M_PI * 2;

  // Calculation of orientated Displacement
  mat3 rotation(1);
  rotation[0][0] = cos(-angle);
  rotation[0][2] = sin(-angle);
  rotation[2][0] = -sin(-angle);
  rotation[2][2] = cos(-angle);
  vec3 oD = rotation * displacement;

  cout << "My oD is " << oD.x << " " << oD.y << " " << oD.z << endl;

  // if statements based on the OD
  // Go forward
  if (oD.z > 0)
  {
    vehicle->accelerate(1000);
    if (abs(oD.x / oD.z) > 0.1)
      vehicle->turn(oD.x / oD.z);
  }
  else
  {
    vehicle->decelerate(1000);
    if (oD.x < 0)
      vehicle->turn(1);
    else
      vehicle->turn(-1);
  }

}
