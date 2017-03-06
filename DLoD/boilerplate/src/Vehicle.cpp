#include "GEO\vehicle\Vehicle.h"



Vehicle::Vehicle()
{
	armour = 30;
	health = 100;	
	dead = false;
	//initialize crash sound and place in map
	crash = Mix_LoadWAV("sfx/carCrash.wav");
	sfxMap.insert(make_pair("crash", crash));
	torqueSpeed = 12000.0;
	colour = vec3(1,0, 0);
}


Vehicle::~Vehicle()
{
	audio.CleanUp();
}

void Vehicle::updateWheelPosition()
{
	float xoff = 1.5;
	float zoff = 4.5;
	mat4 m;
	PxVec3 vCenter = this->physXVehicle->getRigidDynamicActor()->getGlobalPose().p;
	
	//front right
	m = children[0]->getModelMatrix();
	m[3] = vec4(vCenter.x + xoff, 1.0, vCenter.z + zoff, 1.0);
	children[0]->setModelMatrix(m);

	//front left
	m = children[1]->getModelMatrix();
	m[3] = vec4(vCenter.x - xoff, 1.0, vCenter.z + zoff, 1.0);
	children[1]->setModelMatrix(m);

	//back left
	m = children[2]->getModelMatrix();
	m[3] = vec4(vCenter.x - xoff, 1.0, vCenter.z - zoff, 1.0);
	children[2]->setModelMatrix(m);

	//back right
	m = children[3]->getModelMatrix();
	m[3] = vec4(vCenter.x + xoff, 1.0, vCenter.z - zoff, 1.0);
	children[3]->setModelMatrix(m);
}

void Vehicle::accelerate(const float &m)
{
	if (physXVehicle->computeForwardSpeed() < maxVelocity)
	{
		physXVehicle->setDriveTorque(0, m*torqueSpeed);
		physXVehicle->setDriveTorque(1, m*torqueSpeed);
		physXVehicle->setDriveTorque(2, m*torqueSpeed);
		physXVehicle->setDriveTorque(3, m*torqueSpeed);
	}
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
  physXVehicle->setSteerAngle(0, dir / 4);
  physXVehicle->setSteerAngle(1, dir / 4);
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

  updateHealth(damage);
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

float Vehicle::getArmour() {
	return armour;
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
//regenerates armour for vehicle
void Vehicle::regenArmour() {
	armour >= 20 ? armour = 20: armour++;
}

void Vehicle::updateHealth(const float &damage)
{
	float damageToHealth = 0;
	
	if (armour == 0) {
		damageToHealth = damage;
	}
	else {
		//armour takes damage
		armour -= damage;
		//check if there is leftover damage for player
		if (armour < 0) {
			damageToHealth = abs(armour);
			armour = 0;
		}
	}

	health -= damageToHealth;
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
	aliveCar.AddColour(colour);

	if (!deadCar.ReadMesh("models/" + file)) {
		cout << "Error reading destroyed car" << endl;
		return 0;
	}
	deadCar.AddColour(vec3(0, 1, 0));

	//init wheels mesh
	GEO wheel;
	if (!wheel.initMesh("ObjModels/" + wheelFileName)) {
		cout << "Error reading wheel mesh" << endl;
	}

	wheel.getMesh().AddColour(vec3(1, 0, 0));

	mat3 scaleM = mat3(1);
	scaleM[0][0] = scale.x;
	scaleM[1][1] = scale.y;
	scaleM[2][2] = scale.z;

	for (int i = 0; i < wheel.getMesh().vertices.size(); ++i)
		wheel.getMesh().vertices[i] = scaleM * wheel.getMesh().vertices[i];

	for (int i = 0; i < 4; i++) {
		children.push_back(new GEO());
		children[i]->setMesh(wheel.getMesh());
	}

  for (int i = 0; i < aliveCar.vertices.size(); ++i)
    aliveCar.vertices[i] = scaleM * aliveCar.vertices[i];

  for (int i = 0; i < deadCar.vertices.size(); ++i)
    deadCar.vertices[i] = scaleM * deadCar.vertices[i];


	return 1;
}


void Vehicle::changeMeshDead() 
{
	mesh = deadCar;
}

bool Vehicle::initBuffers() {
	if (!aliveCar.Initialize() || !deadCar.Initialize()) {
		return false;
	}
	mesh = aliveCar;
	return true;
}

//returns health string
string Vehicle::getHealthString() {
	string retStr;
	string playerHealth = to_string(int (health));

	switch (playerHealth.size()) {
	case 3:
		retStr = playerHealth;
		break;
	case 2:
		retStr = "0" + playerHealth;
		break;
	case 1:
		retStr = "00" + playerHealth;
		break;
	default:
		retStr = playerHealth;
	}
	return retStr;
}

string Vehicle::getArmourString() {
	string retStr;
	string playerArmour = to_string(int(armour));

	switch (playerArmour.size()) {
	case 3:
		retStr = playerArmour;
		break;
	case 2:
		retStr = "0" + playerArmour;
		break;
	case 1:
		retStr = "00" + playerArmour;
		break;
	default:
		retStr = playerArmour;
	}
	return retStr;
	return retStr;
}