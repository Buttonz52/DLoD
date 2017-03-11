#include "GEO\vehicle\Vehicle.h"



Vehicle::Vehicle()
{
	timer.start();
	armour = 30;
	health = 100;	
	lowHealth = health / 2;
	dead = false;
	canPulseColour = false;
	//initialize crash sound and place in map
	crash = Mix_LoadWAV("sfx/carCrash.wav");
	sfxMap.insert(make_pair("crash", crash));
	filename = "cars/mediumCarBody.obj";
	torqueSpeed = 12000.0;
	maxVelocity = 70;
	colour = vec3(1,0, 0);
	initColour = colour;
}


Vehicle::~Vehicle()
{
	audio.CleanUp();
	if (!timer.isStopped())
		timer.stop();
}

void Vehicle::setColour(const vec3 &col) {
	GEO::setColour(col);
	initColour = col;
}
void Vehicle::updateWheelPosition()
{
	float xoff = 1.5;
	float zoff = 4.5;
	mat4 model;
	PxTransform m = this->physXVehicle->getRigidDynamicActor()->getGlobalPose();
	PxQuat rotate = this->physXVehicle->getRigidDynamicActor()->getGlobalPose().q;
	PxVec3 vCenter = this->physXVehicle->getRigidDynamicActor()->getGlobalPose().p;
	
	mat4 mm = convertMat(m.q.getBasisVector0(), m.q.getBasisVector1(), m.q.getBasisVector2(), vCenter);
	mat4 mRotate = convertMat(rotate.getBasisVector0(), rotate.getBasisVector1(), rotate.getBasisVector2(),PxVec3(0,0,0));
	
	//front right
	model = children[0]->getModelMatrix();
	model[3] = vec4(vCenter.x + xoff, 1.0, vCenter.z + zoff, 1.0);
	model = mm * mRotate;
	children[0]->setModelMatrix(model);

	//front left
	model = children[1]->getModelMatrix();
	model[3] = vec4(vCenter.x - xoff, 1.0, vCenter.z + zoff, 1.0);
	model = mm * mRotate;
	children[1]->setModelMatrix(model);

	//back left
	model = children[2]->getModelMatrix();
	model[3] = vec4(vCenter.x - xoff, 1.0, vCenter.z - zoff, 1.0);
	model = mm * mRotate;
	children[2]->setModelMatrix(model);

	//back right
	model = children[3]->getModelMatrix();
	model[3] = vec4(vCenter.x + xoff, 1.0, vCenter.z - zoff, 1.0);
	model = mm * mRotate;
	children[3]->setModelMatrix(model);
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
	if (physXVehicle->computeForwardSpeed() > -maxVelocity)
	{
		physXVehicle->setDriveTorque(0, m*-torqueSpeed);
		physXVehicle->setDriveTorque(1, m*-torqueSpeed);
		physXVehicle->setDriveTorque(2, m*-torqueSpeed);
		physXVehicle->setDriveTorque(3, m*-torqueSpeed);
	}
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
	if (health <= 0) {
		health = 0;
		if (!dead) {
			if (!timer.isStopped())
				timer.stop();
			mesh.UpdateColour(&(colour = initColour * 0.2f));
			changeMeshDead();
		}
		canPulseColour = false;
		dead = true;
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
	if (armour >= 20.f) {
		//mesh.UpdateColour(&(colour = initColour));
		armour = 20.f;
	}
	else {
		armour++;
	}
			
	//armour >= 20 ? armour = 20: armour++;
}

void Vehicle::updateHealth(const float &damage)
{
	float damageToHealth = 0;
	
	if (armour == 0) {
		//mesh.UpdateColour(&(colour = initColour * 0.5f));
		damageToHealth = damage;
	}
	else {
		//armour takes damage
		armour -= damage;
		//UpdateColour(&(colour = initColour -vec3(0.2f-(armour*0.1f))));
		//check if there is leftover damage for player
		if (armour < 0) {
			damageToHealth = abs(armour);
			armour = 0;
		}
	}

	health -= damageToHealth;
	checkDead();
	
}



bool Vehicle::initMesh(const string &file) {
	aliveCar.AddColour(colour);

	if (!aliveCar.ReadMesh("models/" + filename)) {
		cout << "Error reading alive car" << endl;
		return 0;
	}
	deadCar.AddColour(vec3(0, 1, 0));

	if (!deadCar.ReadMesh("models/" + file)) {
		cout << "Error reading destroyed car" << endl;
		return 0;
	}

	//init wheels mesh
	giveMeWheels();

	mat3 scaleM = mat3(1);
	scaleM[0][0] = scale.x;
	scaleM[1][1] = scale.y;
	scaleM[2][2] = scale.z;

  for (int i = 0; i < aliveCar.vertices.size(); ++i)
    aliveCar.vertices[i] = scaleM * aliveCar.vertices[i];

  for (int i = 0; i < deadCar.vertices.size(); ++i)
    deadCar.vertices[i] = scaleM * deadCar.vertices[i];


	return 1;
}

void Vehicle::giveMeWheels()
{
	mat3 scaleM = mat3(1);
	scaleM[0][0] = scale.x;
	scaleM[1][1] = scale.y;
	scaleM[2][2] = scale.z;

	for (int i = 0; i < 4; i++)
	{
		//init wheels mesh
		GEO* wheel = new GEO();
		wheel->setFilename("wheels/mediumCarTire.obj");
		wheel->setColour(vec3(1, 0, 0));

		if (!wheel->initMesh("wheels/mediumCarTire.obj")) {
			cout << "Error reading wheel mesh" << endl;
		}

		wheel->addShaders("shaders/toon.vert", "shaders/toon.frag");


		if (!wheel->initBuffers()) {
			cout << "Could not initialize buffers for game object " << wheel->getFilename() << endl;
		}

		for (int i = 0; i < wheel->getMesh().vertices.size(); ++i)
			wheel->getMesh().vertices[i] = scaleM * wheel->getMesh().vertices[i];

		children.push_back(wheel);
	}
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
		retStr = playerVelocity;
	}
	return retStr;
}

void Vehicle::Render(const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource)
{
	//make a timer or something so not pulsing crazily
	if (canPulseColour) {
		int denom;
		//check not dividing by 0.  Fiddle with the 6 to find a good timing of pulsation.
		int(ceil(health)/6) <= 0 ? denom = 1 : denom = int(ceil(health)/6);
		//pulse based on how much health you have -> faster pulse - less health
		if ((timer.getTicks() % denom) == 0) {
			colour -= vec3(0.1);
			//if colour = black, reset to initial colour
			if (colour.x <= 0 && colour.y <= 0 && colour.z <= 0)
				colour = initColour;
			mesh.UpdateColour(&colour);
		}
	}
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader.program);
	glBindVertexArray(mesh.vertexArray);

	if (hasTexture) {
		texture.BindTexture(shader.program, "sampler");
	}

	vec3 fp = vec3(0, 0, 0);		//focal point

	mat4 M = getModelMatrix();
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	GLfloat near_plane = 0.001f, far_plane = 1000.f;
	//lightProjection = glm::ortho(-600.0f, 600.0f, -600.0f, 600.0f, near_plane, far_plane);
	lightProjection = glm::ortho(-1000.f, 1000.f, -1000.f, 1000.f, near_plane, far_plane);

	//lightProjection = glm::perspective(float(M_PI / 3),1920.f/1080.f,near_plane, far_plane);
	lightView = glm::lookAt(_lightSource, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

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

	// check for an report any OpenGL errors
	CheckGLErrors();

	for (int i = 0; i < children.size(); i++)		//GEO* child : children)
		children[i]->Render(_view, _projection, _lightSource);
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


