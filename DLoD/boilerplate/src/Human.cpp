#include "GEO\player\Human.h"

Human::Human(int i, Audio *audio) : Player(i)
{
	type = human;
	this->audio = audio;
  camIndex = 0;
  numCams = 1;
  controller = new XboxController(i+1);
  restart = false;
  pausePressed = false;
}


Human::~Human()
{
  delete controller;
  delete click;
}

void Human::setNumCams(const int &n) {
	numCams = n;
}

int Human::getNumCams() {
	return numCams;
}
// FIX THIS
void Human::getInput(GLFWwindow* window, bool &pause)
{
  if (vehicle->stun.first) {
    if (vehicle->timer.getTicks() < vehicle->stun.second)
      return;
    else {
      vehicle->stun = make_pair(false, 0);
    }
  }
  if (!pause)
	  vehicleControls(window, pause);
}

void Human::getGameOverInput(GLFWwindow* window, bool &pause)
{
	gameOverControls(window, pause);
}

void Human::gameOverControls(GLFWwindow* window, bool &pause)
{
	if (!controller->Connected() && identifier == 0)
	{
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			pause = false;
		}
	}
}

// handles keyboard input events when we want multiple keys pressed at once
void Human::vehicleControls(GLFWwindow* window, bool &pause)
{
  int state;

  //Movement of the GEOs with keyboard if controller not connected
  //make it so that only player 1 can use keyboard if no controller
  if (!controller->Connected() && identifier ==0)
  {
	  if (!isDead()) {
		  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		  {
			  vehicle->accelerate(1);
		  }
		  else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		  {
			  vehicle->decelerate(1);
		  }
		  else {
			  vehicle->brake(10000.f);
		  }
		  state = glfwGetKey(window, GLFW_KEY_LEFT);
		  if (state == GLFW_PRESS)
		  {
			  vehicle->turn(-1);
		  }
		  state = glfwGetKey(window, GLFW_KEY_RIGHT);
		  if (state == GLFW_PRESS)
		  {
			  vehicle->turn(1);
		  }
		  state = glfwGetKey(window, GLFW_KEY_A);
		  if (state == GLFW_PRESS)
		  {
			  layTrap = true;
			  trap = DamageTrap;
			  audio->PlaySfx("throwDmg", MIX_MAX_VOLUME / 2, throwTrap);
		  }
		  state = glfwGetKey(window, GLFW_KEY_S);
		  if (state == GLFW_PRESS)
		  {
			  layTrap = true;
			  trap = EmpTrap;
			  audio->PlaySfx("throwStun", MIX_MAX_VOLUME / 2, throwTrap);

		  }

		  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			  audio->PlaySfx("horn", MIX_MAX_VOLUME/2, select);
			  vehicle->FlipVehicle();
		  }

	  }
	  //switch between cameras only if dead
	  else {
		  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
			  camIndex >= numCams ? camIndex = 0 : camIndex++;
			  Sleep(300);
		  }
	  }
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			pausePressed = true;
			pause = true;
			Sleep(300);
		}

  }
  else {
    controller->Update();
    float turn;
	if (!isDead()) {
		if (controller->RightTrigger() != 0)
		{
			vehicle->accelerate(controller->RightTrigger());
		}

		else if (controller->LeftTrigger() != 0)
		{
			vehicle->decelerate(controller->LeftTrigger());
		}
		else
		{
			vehicle->brake(10000.f);
		}
		if (controller->LeftStick_X() > -0.25 && controller->LeftStick_X() < 0.25)
			turn = 0.0;
		else
			turn = controller->LeftStick_X();

		//lay damage trap
		if (controller->GetButtonPressed(XBtns.DPad_Left) || controller->GetButtonPressed(XBtns.R_Shoulder)) {
			layTrap = true;
			trap = DamageTrap;
			audio->PlaySfx("throwDmg", MIX_MAX_VOLUME / 2, throwTrap);

		}
		//lay empty trap
		if (controller->GetButtonPressed(XBtns.DPad_Up) || controller->GetButtonPressed(XBtns.L_Shoulder)) {
			layTrap = true;
			trap = EmpTrap;
			audio->PlaySfx("throwStun", MIX_MAX_VOLUME / 2, throwTrap);

		}
		//flip car
		if (controller->GetButtonPressed(XBtns.Y)) {
			audio->PlaySfx("horn", MIX_MAX_VOLUME/2, throwTrap);
				vehicle->FlipVehicle();
		}
		vehicle->turn(turn);
	}
	else {
		//just stop
		 vehicle->brake(10000.0);
		
		//switch between cameras only if dead
		if (controller->GetButtonPressed(XBtns.BackBtn)) {
			camIndex >= numCams ? camIndex = 0 : camIndex++;
			Sleep(300);
		}
	}
	  if (controller->GetButtonPressed(XBtns.StartBtn)) {
		  pausePressed = true;
		  pause = true;
		  Sleep(300);
	  }
  }
}

bool Human::pressedPause() {
	return pausePressed;
}
bool Human::restartGame() {
	return restart;
}

