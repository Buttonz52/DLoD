#include "Game\Video.h"
#include <Windows.h>

Video::Video()
{
	videoinit = SDL_FALSE;
}


Video::~Video()
{
}

void Video::onQuit() {
	if (videoinit) {
		SDL_VideoQuit();
	}
	SDL_Quit();
}

void Video::InitializeVideo() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Error initializing video" << endl;
	}
	if (SDL_VideoInit(NULL) != 0) {
		cout << "Error initializing video" << endl;
	}
	
	videoinit = SDL_TRUE;
	GLFWwindow *currentWindow;
	currentWindow = glfwGetCurrentContext();
	const GLFWvidmode* desktop;
	int x, y, s,t;
	glfwGetWindowSize(currentWindow, &x,&y);
	glfwGetWindowPos(currentWindow, &s,&t);
	desktop = glfwGetVideoMode(glfwGetPrimaryMonitor());
	
	SDL_CreateWindow("hi", s,t, x,y, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

}