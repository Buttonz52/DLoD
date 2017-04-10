#pragma once
#include <SDL.h>
#include "Utility.h"
using namespace std;
class Video
{
public:
	Video();
	~Video();
	void onQuit();

	void InitializeVideo();

	SDL_bool videoinit;
};

