#include "GEO\vehicle\Vehicle.h"



Vehicle::Vehicle()
{
	health = 10;	//set to 50 for debugging stuff
	dead = false;
	//initialize crash sound and place in map
	crash = Mix_LoadWAV("sfx/carCrash.wav");
	sfxMap.insert(make_pair("crash", crash));

}


Vehicle::~Vehicle()
{
	audio.CleanUp();
}


void Vehicle::accelerate(const float &m)
{
  physXVehicle->setDriveTorque(0, m*torqueSpeed);
  physXVehicle->setDriveTorque(1, m*torqueSpeed);
  physXVehicle->setDriveTorque(2, m*torqueSpeed);
  physXVehicle->setDriveTorque(3, m*torqueSpeed);
}


void Vehicle::decelerate(const float &m)
{
  physXVehicle->setDriveTorque(0, m*-torqueSpeed);
  physXVehicle->setDriveTorque(1, m*-torqueSpeed);
  physXVehicle->setDriveTorque(2, m*-torqueSpeed);
  physXVehicle->setDriveTorque(3, m*-torqueSpeed);
}

void Vehicle::turn(const float &dir)
{
  physXVehicle->setSteerAngle(0, dir / 5);
  physXVehicle->setSteerAngle(1, dir / 5);
}

void Vehicle::brake(const float &brake)
{
  physXVehicle->setBrakeTorque(0, brake);
  physXVehicle->setBrakeTorque(1, brake);
  physXVehicle->setBrakeTorque(2, brake);
  physXVehicle->setBrakeTorque(3, brake);
}

void Vehicle::releaseAllControls()
{
  physXVehicle->setDriveTorque(0, 0.0f);
  physXVehicle->setDriveTorque(1, 0.0f);
  physXVehicle->setDriveTorque(2, 0.0f);
  physXVehicle->setDriveTorque(3, 0.0f);

  physXVehicle->setBrakeTorque(0, 0.0f);
  physXVehicle->setBrakeTorque(1, 0.0f);
  physXVehicle->setBrakeTorque(2, 0.0f);
  physXVehicle->setBrakeTorque(3, 0.0f);

  physXVehicle->setSteerAngle(0, 0.0f);
  physXVehicle->setSteerAngle(1, 0.0f);
  physXVehicle->setSteerAngle(2, 0.0f);
  physXVehicle->setSteerAngle(3, 0.0f);
}

float Vehicle::calculateDamage(const double &x, const double &y, const double &z, const double &force)
{
  // get the position of the vehicle
  mat4 M = getModelMatrix();
  vec3 pos = vec3(M[3]);

  vec3 displacement = vec3(x, y, z) - pos;

  // Get the rotation of the object
  physx::PxVec3 axis = physx::PxVec3(0, 1, 0);
  physx::PxReal angle = 0;
  physXVehicle->getRigidDynamicActor()->getGlobalPose().q.toRadiansAndUnitAxis(angle, axis);

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

  double damage;

  // Curently some arbitrary multipliers are in here it should change at some point
  // Some fine tuning is needed
  // damage to the front or back
  if (abs(oD.z) > abs(oD.x))
  {
    // damage to the front
    if (oD.z > 0)
    {
      damage = force * 0.3;
    }
    // damage to the back
    else
    {
      damage = force;
    }
  }
  // damage to the sides
  else
  {
    // damage to the right
    if (oD.x > 0)
    {
      damage = force * 0.6;
    }
    // damage to the left
    else
    {
      damage = force * 0.6;
    }
  }

  health -= damage;
  checkDead();
  return damage;

}

bool Vehicle::isDead() {
	return dead;
}
float Vehicle::getHealth()
{
	return health;
}

void Vehicle::checkDead() {
	if (health < 0) {
		health = 0;
		if (!dead) {
			changeMeshDead();

		}
		dead = true;
	}
}
void Vehicle::updateHealth(const float &damage)
{
	health -= damage;
	if (health < 0) {
		health = 0;
		if (!dead) {
			changeMeshDead();

		}
		dead = true;
	}
}

bool Vehicle::initMesh(const string &file) {

	if (!aliveCar.ReadMesh("models/" + filename)) {
		cout << "Error reading alive car" << endl;
		return 0;
	}
	aliveCar.AddColour(vec3(1, 0, 0));
	//if (!aliveCar.Initialize()) {
	//	return 0;
	//}

	if (!deadCar.ReadMesh("models/" + file)) {
		cout << "Error reading destroyed car" << endl;
		return 0;
	}
	deadCar.AddColour(vec3(0, 1, 0));
#if DEBUG
	cout << "Number of verts: " << deadCar.vertices.size() << endl;
#endif
	//if (!destroyedCar.Initialize()) {
	//	return 0;
	//}
	//cout << "number of verts: " << mesh.vertices.size() << endl;
	//cout << "Loaded " << filename << endl;
	//	cout << "number of verts destroyed car: " << destroyedCar.vertices.size() << endl;
	//	cout << "Loaded destroyed car: " << file << endl;
	return 1;
}

void Vehicle::changeMeshDead() {
	mesh = deadCar;
	//mesh.Initialize();
	//if (!initBuffers()) {
	//	cout << "Could not initialize buffers for destroyed car" << endl;
	//}
}
bool Vehicle::initBuffers() {
	if (!aliveCar.Initialize() || !deadCar.Initialize()) {
		return false;
	}
	mesh = aliveCar;
	return true;
}