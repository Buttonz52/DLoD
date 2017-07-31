#pragma once
#include "Utility.h"
#include "GameEngine/Mesh.h"
#include "ScreenOverlay.h"
#include "Controller/XboxController.h"
#include "Audio.h"
#include "GEO/GEO.h"
class GameHud
{
public:
	enum gameMode {
		time, sudden_death
	};

	GameHud();
	GameHud(const gameMode & version);
	~GameHud();

	void setMode(const gameMode & version);

	//updates positions of cars to render to radar portion of hud
	void UpdateRadar(const vector<vec3>* positions, const vector<vec3> *colours);

	//initializes hud
	void InitializeHud(const vec3 &colour, const vector<vec3>*positions, const string &reanaFilename);

	void Render(vector<string>& hudStrings, const vector<vec3>* positions, const vec3 & colour, const vector<vec3>* colours, const bool & canLayTrap);

private:
	gameMode mode;
	//widgets of game hud
	ScreenOverlay	healthTex, armourTex,
		timeTex,
		healthTitle, armourTitle,
		topLeftBkgrd, topLeftBorder,
		botRightBorder, botRightBkgrd,
		velocityTitle, velocityTex,
		radarBkgrd, radarBorder,
		radarPoints, screenBorder,
		dpadTexture, dpadBorder,
		weaponUpD, weaponLeftD;
	;
};

