#pragma once
#include "Utility.h"
#include "../GameEngine/Mesh.h"
#include "ScreenOverlay.h"
#include "../Controller/XboxController.h"
#include "Audio.h"
class GameHud
{
public:
	GameHud();
	~GameHud();

	//updates positions of cars to render to radar portion of hud
	void UpdateRadar(const vector<vec3>* positions);
	
	//initializes hud
	void InitializeHud(const vec3 &colour, const vector<vec3>*positions, const string &reanaFilename);

	//renders all widgets
	void Render(const string &health, const string &armour, const string &velocity, const vector<vec3>*positions);

private:
	//widgets of game hud
	ScreenOverlay	healthTex, armourTex, 
					healthTitle, armourTitle, 
					topLeftBkgrd, topLeftBorder, 
					botRightBorder, botRightBkgrd,
					velocityTitle, velocityTex,
					radarBkgrd,	radarBorder,
					radarPoints;
};

