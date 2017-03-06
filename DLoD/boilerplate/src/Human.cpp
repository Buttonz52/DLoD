#include "GEO\player\Human.h"



Human::Human(int i) : Player(i)
{
  controller = new XboxController(1);
}


Human::~Human()
{
  delete controller;
}


// FIX THIS
void Human::getInput(GLFWwindow* window)
{
  vehicleControls(window);
}

void Human::getGameOverInput(GLFWwindow* window, bool &pause)
{
	gameOverControls(window, pause);
}

void Human::gameOverControls(GLFWwindow* window, bool &pause)
{
	if (!controller->Connected())
	{
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			pause = false;
		}
	}
}

// handles keyboard input events when we want multiple keys pressed at once
void Human::vehicleControls(GLFWwindow* window)
{
  int state;

  //Movement of the GEOs with keyboard if controller not connected
  if (!controller->Connected())
  {
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
    state = glfwGetKey(window, GLFW_KEY_SPACE);
    if (state == GLFW_PRESS)
    {
      //audio.PlaySfx(audio.horn);
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

      vehicle->turn(turn);
    }
    else {	//just stop
      vehicle->brake(1000.0);
    }
  }
}

void Human::menuControls(GLFWwindow* window)
{

}
