#ifndef KEYCALLBACK_H
#define KEYCALLBACK_H

#include "Utility.h"

void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif