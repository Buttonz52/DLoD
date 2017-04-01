#include "GEO\player\Human.h"



Human::Human(int i) : Player(i)
{
  camIndex = 0;
  numCams = 1;
  controller = new XboxController(i+1);
  restart = false;
  pausePressed = false;
  click = Mix_LoadWAV("sfx/bubblePop.wav");
  back = Mix_LoadWAV("sfx/backSfx.wav");
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
      vehicle->timer.stop();
      vehicle->timer.reset();
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
			  vehicle->brake(8000.f);
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
		  }
		  state = glfwGetKey(window, GLFW_KEY_S);
		  if (state == GLFW_PRESS)
		  {
			  layTrap = true;
			  trap = EmpTrap;
		  }

		  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			  if (ableToFlip)
				  vehicle->FlipVehicle();
			  //	ableToFlip = false;
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
	if (!vehicle->isDead()) {
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
			vehicle->brake(1000.f);
		}
		if (controller->LeftStick_X() > -0.25 && controller->LeftStick_X() < 0.25)
			turn = 0.0;
		else
			turn = controller->LeftStick_X();

		//lay damage trap
		if (controller->GetButtonPressed(XBtns.X) || controller->GetButtonPressed(XBtns.DPad_Up)) {

			layTrap = true;
			trap = DamageTrap;
		}
		//lay empty trap
		if (controller->GetButtonPressed(XBtns.Y) || controller->GetButtonPressed(XBtns.DPad_Left)) {

			layTrap = true;
			trap = EmpTrap;
		}
		vehicle->turn(turn);
	}
	else {
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

    else {	//just stop
      vehicle->brake(1000.0);
    }
  }
}

void Human::menuControls(GLFWwindow* window, bool &pause, int &index, Audio *audio)
{
	if (!controller->Connected()) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			pause = false;
			pausePressed = false;
			audio->PlaySfx(back, MIX_MAX_VOLUME,1);
			Sleep(300);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			index -= 1;
			audio->PlaySfx(click, MIX_MAX_VOLUME,1);
			Sleep(150);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			index += 1;
			audio->PlaySfx(click, MIX_MAX_VOLUME,1);
			Sleep(150);
		}
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			menuItemPressed = true;
			pausePressed = false;
			pause = false;
			audio->PlaySfx(click, MIX_MAX_VOLUME,1);
			Sleep(300);

		}
	}
	else {
		if (controller->GetButtonPressed(XBtns.StartBtn)) {
			pause = false;
			pausePressed = false;
			audio->PlaySfx(back, MIX_MAX_VOLUME,1);
			Sleep(300);
		}
		if (controller->GetButtonPressed(XBtns.DPad_Up)) {
			index -= 1;
			audio->PlaySfx(click, MIX_MAX_VOLUME,1);
			Sleep(150);
		}
		if (controller->GetButtonPressed(XBtns.DPad_Down)) {
			index += 1;
			audio->PlaySfx(click, MIX_MAX_VOLUME,1);
			Sleep(150);
		}
		if (controller->GetButtonPressed(XBtns.A)) {
			menuItemPressed = true;
			pausePressed = false;
			pause = false;
			audio->PlaySfx(click, MIX_MAX_VOLUME,1);
			Sleep(300);
		}
	}
}

bool Human::MenuItemSelected() {
	return menuItemPressed;
}
bool Human::pressedPause() {
	return pausePressed;
}
bool Human::restartGame() {
	return restart;
}