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
	GameHud();
	~GameHud();

	//updates positions of cars to render to radar portion of hud
	void UpdateRadar(const vector<vec3>* positions, const vector<vec3> *colours);

	void InitializeMenu(const vec3 & colour);

	void InitializeEndGame(const vector<string>& playerNames, const vector<vec3>& colours);
	
	//initializes hud
	void InitializeHud(const vec3 &colour, const vector<vec3>*positions, const string &reanaFilename);

	//renders all widgets
	//void Render(const string &health, const string &armour, const string &velocity, const vector<vec3>*positions, const vec3 &colour);

	//void Render(const string & health, const string & armour, const string & velocity, const vector<vec3>* positions, const vec3 & colour, const bool & canLayTrap);

	void Render(const string & health, const string & armour, const string & velocity, const vector<vec3>* positions, const vec3 & colour, const vector<vec3>* colours, const bool & canLayTrap);

	void RenderEndGame(const int & menuIndex, const vec3 & colour);

	void RenderMenu(const int & menuIndex, const vec3 & colour);

	//void RenderMenu(const int & menuIndex);

private:
	//widgets of game hud
	ScreenOverlay	healthTex, armourTex,
		healthTitle, armourTitle,
		topLeftBkgrd, topLeftBorder,
		botRightBorder, botRightBkgrd,
		velocityTitle, velocityTex,
		radarBkgrd, radarBorder,
		radarPoints, screenBorder,
		resumeText, quitText,
		pauseBkgrd, pauseBox,
		pauseBorder, restartText,
		dpadTexture, dpadBorder,
		weaponUpD, weaponLeftD,
		firstPlace, secondPlace,
		thirdPlace, fourthPlace,
		endGameOverlay;
	;
};

