#include "GEO\vehicle\Vehicle.h"



Vehicle::Vehicle()
{
	timer.start();
	initArmour = 30;
	armour = 30;
	health = 100;	
	lowHealth = health / 2.f;
	dead = false;
	canPulseColour = false;

	//initialize crash sound and place in map
	crash = Mix_LoadWAV("sfx/carCrash.wav");
	explosion = Mix_LoadWAV("sfx/explosion.wav");
	sfxMap.insert(make_pair("crash", crash));
	sfxMap.insert(make_pair("explosion", explosion));

	filename = "cars/mediumCarBody.obj";
	armourFilename = "armour/MediumArmour.obj";
	torqueSpeed = 18000.0;
	colour = vec3(1,0, 0);
	initColour = colour;
	reflectance = 0.3f;
	timeOfDeath = -1;
	flipTime = 0;
	ableToFlip = true;
}


Vehicle::~Vehicle()
{
	audio.CleanUp();
	if (!timer.isStopped())
		timer.stop();
}
int Vehicle::getTimeOfDeath() {
	return timeOfDeath;
}

void Vehicle::setColour(const vec3 &col) {
	GEO::setColour(col);
	initColour = col;
}

vec3 Vehicle::getOrientationVector()
{
  // get the position of the vehicle
  mat4 M = getModelMatrix();
  vec3 pos = vec3(M[3]);

  // Get the rotation of the object
  physx::PxVec3 axis = physx::PxVec3(0, 1, 0);
  physx::PxReal angle = 0;
  physXVehicle->getRigidDynamicActor()->getGlobalPose().q.toRadiansAndUnitAxis(angle, axis);

  int fix = (axis.y < 0) ? 1 : -1;
  angle *= fix;
  if (angle < 0)
    angle += M_PI * 2;

  mat3 rotation(1);
  rotation[0][0] = cos(-angle);
  rotation[0][2] = sin(-angle);
  rotation[2][0] = -sin(-angle);
  rotation[2][2] = cos(-angle);

  return rotation * vec3(0.0, 0.0, 1.0);
}

void Vehicle::updateArmour() {
	children[4]->transparency = (armour / initArmour);

	PxTransform m = this->physXVehicle->getRigidDynamicActor()->getGlobalPose();
	PxVec3 vCenter = this->physXVehicle->getRigidDynamicActor()->getGlobalPose().p;
	mat4 mm = convertMat(m.q.getBasisVector0(), m.q.getBasisVector1(), m.q.getBasisVector2(), vCenter);
	children[4]->setModelMatrix(mm * glm::scale(scale));
}
double Vehicle::getInitialArmour()
{
  return initArmour;
}
void Vehicle::updateWheelPosition()
{

	mat4 model;
	PxTransform m = this->physXVehicle->getRigidDynamicActor()->getGlobalPose();
	PxQuat rotate = this->physXVehicle->getRigidDynamicActor()->getGlobalPose().q;
	PxVec3 vCenter = this->physXVehicle->getRigidDynamicActor()->getGlobalPose().p;

	PxVec3 displacement(xoff, 0, zoff);
	PxTransform name = m;

	displacement = rotate.rotate(displacement) + rotate.rotate(PxVec3(0.0, 0.0, centeroff));
	name.p += displacement;
	model = convertMat(name.q.getBasisVector0(), name.q.getBasisVector1(), name.q.getBasisVector2(), vCenter + displacement);
	children[0]->setModelMatrix(model);

	displacement = PxVec3(-xoff, 0, zoff);

	name = m;
	displacement = rotate.rotate(displacement) + rotate.rotate(PxVec3(0.0, 0.0, centeroff));

	name.p += displacement;

	model = convertMat(name.q.getBasisVector0(), name.q.getBasisVector1(), name.q.getBasisVector2(), vCenter + displacement);
	children[1]->setModelMatrix(model);


	displacement = PxVec3(xoff, 0, -zoff);
	name = m;
	displacement = rotate.rotate(displacement) + rotate.rotate(PxVec3(0.0, 0.0, centeroff));

	name.p += displacement;

	model = convertMat(name.q.getBasisVector0(), name.q.getBasisVector1(), name.q.getBasisVector2(), vCenter + displacement);
	children[2]->setModelMatrix(model);


	displacement = PxVec3(-xoff, 0, -zoff);
	name = m;
	displacement = rotate.rotate(displacement) + rotate.rotate(PxVec3(0.0, 0.0, centeroff));

	name.p += displacement;

	model = convertMat(name.q.getBasisVector0(), name.q.getBasisVector1(), name.q.getBasisVector2(), vCenter + displacement);
	children[3]->setModelMatrix(model);
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
  physXVehicle->setSteerAngle(2, -dir / 4.5);
  physXVehicle->setSteerAngle(3, -dir / 4.5);
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
      damage = force * 0.15;
    }
    // damage to the back
    else
    {
      damage = force*0.4;
    }
  }
  // damage to the sides
  else
  {
    // damage to the right
    if (oD.x > 0)
    {
      damage = force * 0.3;
    }
    // damage to the left
    else
    {
      damage = force * 0.3;
    }
  }

  updateHealth(damage);
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

void Vehicle::repair(const float & armourAdded)
{
  armour += armourAdded;
}

void Vehicle::checkDead() {
	if (health <= 0) {
		health = 0;
		
		if (!dead) {
			timeOfDeath = timer.getTicks();
			if (!timer.isStopped())
				timer.stop();
			mesh.UpdateColour(vec3());
		}
		canPulseColour = false;
		dead = true;
		mesh.UpdateColour(vec3());
	}
	else if (health >= lowHealth) {
		canPulseColour = false;
	}
	else {
		canPulseColour = true;
	}
}
//regenerates armour for vehicle
void Vehicle::regenArmour() {
	if (armour >= initArmour) {
		//armour = initArmour;
	}
	else {
		armour++;
	}
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
	checkDead();
}

void Vehicle::heal(const float & healthAdded)
{
  health = min(health + healthAdded, initHealth);
}

bool Vehicle::initMesh(const string &file) {
	mesh.AddColour(colour);

	if (!mesh.ReadMesh("models/" + filename)) {
		//cout << "Error reading alive car" << endl;
		return 0;
	}

	//init wheels mesh
	giveMeWheels();
	giveMeArmour(colour);
	mat3 scaleM = mat3(1);
	scaleM[0][0] = scale.x;
	scaleM[1][1] = scale.y;
	scaleM[2][2] = scale.z;

	for (int i = 0; i < mesh.vertices.size(); ++i)
		mesh.vertices[i] = scaleM * mesh.vertices[i];

	return 1;
}

void Vehicle::giveMeArmour(const vec3 &colour) {
	GEO *armourGEO = new GEO();

	mat3 scaleM = mat3(1);
	scaleM[0][0] = scale.x;
	scaleM[1][1] = scale.y;
	scaleM[2][2] = scale.z;

	float greyscale = 0.299 * colour.x + 0.587 * colour.y + 0.114 * colour.z;
	//armourGEO->setColour(colour);
	armourGEO->setColour(vec3(greyscale));

	if (!armourGEO->initMesh(armourFilename)) {
	//	cout << "Error reading armour mesh." << endl;
		return;
	}
	armourGEO->addShaders("shaders/toon.vert", "shaders/toon.frag");

	if (!armourGEO->initBuffers()) {
	//	cout << "Could not initialize buffers for game object " << filename << endl;
	}


	for (int i = 0; i <armourGEO->getMesh().vertices.size(); ++i) {
		armourGEO->getMesh().vertices[i] = scaleM * armourGEO->getMesh().vertices[i];

	}
	armourGEO->setEnvironmentMap(environmentMap);
	armourGEO->setExposure(1.f);
	armourGEO->setReflectance(1.f);
	children.push_back(armourGEO);
}
void Vehicle::giveMeWheels()
{
	float wScale = 3.f;
	mat3 scaleM = mat3(1);
	scaleM[0][0] = wScale;
	scaleM[1][1] = wScale;
	scaleM[2][2] = wScale;

	for (int i = 0; i < 4; i++)
	{
		//init wheels mesh
		GEO* wheel = new GEO();
		wheel->setColour(vec3(0, 0, 0));
		string filename = "wheels/mediumCarTire.obj";
		if (!wheel->initMesh(filename)) {
		//	cout << "Error reading wheel mesh" << endl;
		}

		wheel->addShaders("shaders/toon.vert", "shaders/toon.frag");

		for (int i = 0; i < wheel->getMesh().vertices.size(); ++i)
			wheel->getMesh().vertices[i] = scaleM * wheel->getMesh().vertices[i];

		if (!wheel->initBuffers()) {
		//	cout << "Could not initialize buffers for game object " << filename << endl;
		}

		children.push_back(wheel);
	}
}

bool Vehicle::initBuffers() {
	if (!mesh.Initialize()) {
		return false;
	}
	//mesh = aliveCar;
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

string Vehicle::getVelocityString() {
	string retStr;
	PxVec3 velocity = physXVehicle->getRigidDynamicActor()->getLinearVelocity();
	string playerVelocity = to_string(int(velocity.magnitude()));

	switch (playerVelocity.size()) {
	case 2:
		retStr = playerVelocity;
		break;
	case 1:
		retStr = "0" + playerVelocity;
		break;
	default:
    //else, assuming the size is >3 
		//retStr = playerVelocity;
    retStr = "99";
	}
	return retStr;
}

void Vehicle::Render(const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource)
{
	//make a timer or something so not pulsing crazily
	if (canPulseColour) {
		int denom;
		int healthCompare = int(ceil(health) / 6.f);
		//check not dividing by 0.  Fiddle with the 6 to find a good timing of pulsation.
		healthCompare <= 0 ? denom = 1 : denom = healthCompare;
		//pulse based on how much health you have -> faster pulse - less health
		if ((timer.getTicks() % denom) == 0) {
			colour -= vec3(0.1);
			//if colour = black, reset to initial colour
			if (colour.x <= 0 && colour.y <= 0 && colour.z <= 0)
				colour = initColour;
			mesh.UpdateColour(colour);
		}
	}

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader.program);
	glBindVertexArray(mesh.vertexArray);

	if (hasTexture) {
		texture.BindTexture(shader.program, "sampler");
	}

	if (hasEnvMap) {
		environmentMap.BindTexture(shader.program, "radiancemap");
	}
	vec3 fp = vec3(0, 0, 0);		//focal point

	mat4 M = getModelMatrix();

	glUniform1f(glGetUniformLocation(shader.program, "exposure"), exposure);
	glUniform1f(glGetUniformLocation(shader.program, "reflectance"), reflectance);

	glUniform1f(glGetUniformLocation(shader.program, "transparency"), transparency);
	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, value_ptr(M));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "modelview"), 1, GL_FALSE, value_ptr(_view));

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, value_ptr(_projection));
	glUniform3fv(glGetUniformLocation(shader.program, "lightPosition"), 1, value_ptr(_lightSource));

	glDrawElements(GL_TRIANGLES, mesh.elementCount, GL_UNSIGNED_SHORT, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	if (hasTexture)
		texture.UnbindTexture();

	if (hasEnvMap)
		environmentMap.UnbindTexture();
	// check for an report any OpenGL errors
	CheckGLErrors();

	for (int i = 0; i < children.size(); i++) {	//GEO* child : children)
		if (i == 4 && dead) {
			continue;
		}
		children[i]->Render(_view, _projection, _lightSource);

	}
}

mat4 Vehicle::convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w)
{
	mat4 M = mat4(x.x, y.x, z.x, w.x,
		x.y, y.y, z.y, w.y,
		x.z, y.z, z.z, w.z,
		0.0, 0.0, 0.0, 1.0f);

	M = glm::transpose(M);

	return M;
}

//Resets flip boolean
void Vehicle::checkEnableFlip() {
	//if timer is past flip time + value, then flip car over
	if (!ableToFlip) {
		if (timer.getTicks() > flipTime + clock_t(1000)) {
			ableToFlip = true;
		}
	}
}
//Flips vehicle over
void Vehicle::FlipVehicle() {
	float armourPenalty = 5.f;
	//checks if you are able to flip
	checkEnableFlip();
	if (ableToFlip) {
		ableToFlip = false;
	
		//get base value for timer to enable flipping again
		flipTime = timer.getTicks();
		float force = 1000;
		float torque = 1000;

		// Get the rotation of the object
		float mass = physXVehicle->getRigidDynamicActor()->getMass();
		PxVec3 axis(0, 0, 1);
		axis = physXVehicle->getRigidDynamicActor()->getGlobalPose().rotate(axis);
		//cout << axis.x << " " <<axis.y << " " <<axis.z << endl;
		physXVehicle->getRigidDynamicActor()->addForce(PxVec3(0, force*mass, 0));
		physXVehicle->getRigidDynamicActor()->addTorque(axis*torque*mass);

		//armour penalty because of flipping
		//armour takes damage
		armour -= armourPenalty;
		if (armour < 0) {
			armour = 0;
		}
	}
}