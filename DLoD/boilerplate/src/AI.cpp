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

  // Calculation the normalized orientated displacement
  mat3 rotation(1);
  rotation[0][0] = cos(-angle);
  rotation[0][2] = sin(-angle);
  rotation[2][0] = -sin(-angle);
  rotation[2][2] = cos(-angle);
  vec3 oD = rotation * displacement;
  oD = normalize(oD);



  /* If you want to do some fine tuning of the AI change 
   * the code below.
   */

  if (oD.z > 0)
  {
    vehicle->accelerate(5);

    double turn = min(max(-oD.x / oD.z, -0.6), 0.6);
    turn = ((vehicle->physXVehicle->computeForwardSpeed() > 0)) ? turn : -turn;

    vehicle->turn(turn);
  }
  else
  {
    vehicle->decelerate(5);
    
    double turn = min(max(-oD.x / oD.z, -0.7), 0.7);
    turn = ((vehicle->physXVehicle->computeForwardSpeed() < 0)) ? turn : -turn;

    vehicle->turn(turn);
  }

  if (length(displacement) < 5)
  {
    vehicle->brake(10000.0);
    cout << "breaking" << endl;
  }

}
