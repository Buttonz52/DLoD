#include "Game\GameHud.h"

//Game Hud class.

GameHud::GameHud()
{
}


GameHud::~GameHud()
{
}


void GameHud::InitializeMenu(const vec3 &colour) {
	int kerning = 30;
	resumeText.InitializeGameText("RESUME", vec3(-0.27, 0.0, 0), colour, kerning);
	resumeText.setScale(vec3(2));
	restartText.InitializeGameText("RESTART", vec3(-0.3, -0.2, 0), colour, kerning);
	restartText.setScale(vec3(2));
	quitText.InitializeGameText("QUIT", vec3(-0.2, -0.4, 0), colour, kerning);
	quitText.setScale(vec3(2));

	//arena map background
	if (!pauseBkgrd.initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}

	pauseBkgrd.GenerateSquareVertices(1, 1, vec3(0.6, 0.5, 1));
	pauseBkgrd.setMixFlag(1);
	pauseBkgrd.setMixAmount(0.5);
	pauseBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	
	//border for health and armour
	pauseBox.GenerateSquareVertices(0.25, 0.32, vec3(0.6, 0.7, 1));
	pauseBox.setPosition(vec3(-0.055, -0.07, 0));
	pauseBox.setTransparency(0.5f);
	pauseBox.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	pauseBorder.GenerateBorder(0.25, 0.32, 0.01, vec3(0, 0, 0), vec3(-0.055, -0.07, 0));
	pauseBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

}
void GameHud::InitializeEndGame(const vector<string> &playerNames, const vector <vec3> &colours) {
	quitText.setPosition(vec3(-0.19, -0.9, 0));
	restartText.setPosition(vec3(-0.29, -0.7, 0));
	std::stringstream fmt1;
	int kerning = 30;
	fmt1 << "1st place: " << playerNames[0];
	firstPlace.InitializeGameText(fmt1.str(), vec3(-0.35, 0.3, 0), colours[0], kerning);
	std::stringstream fmt2;
	fmt2 << "2nd place: " << playerNames[1];
	secondPlace.InitializeGameText(fmt2.str(), vec3(-0.35, 0.1, 0), colours[1], kerning);
	std::stringstream fmt3;
	fmt3 << "3rd place: " << playerNames[2];
	thirdPlace.InitializeGameText(fmt3.str(), vec3(-0.35, -0.1, 0), colours[2], kerning);
	std::stringstream fmt4;
	fmt4 << "4th place: " << playerNames[3];
	fourthPlace.InitializeGameText(fmt4.str(), vec3(-0.35, -0.3, 0), colours[3], kerning);
	endGameOverlay.GenerateSquareVertices(0.5, 0.5, vec3(1));
	endGameOverlay.setTransparency(0.5f);

	endGameOverlay.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	firstPlace.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	secondPlace.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	thirdPlace.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	fourthPlace.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}
//initialize hud
void GameHud::InitializeHud(const vec3 &colour, const vector<vec3> *positions, const string &arenaFilename) {
	int kerning = 30;
	//health
	healthTitle.InitializeGameText("Health:", vec3(-0.98, 0.8, 0), colour, kerning);
	healthTitle.setScale(vec3(1.f));

	healthTex.InitializeGameText("000", vec3(-0.73, 0.8, 0), colour, kerning);
	healthTex.setScale(vec3(1.2f));

	//armour
	armourTitle.InitializeGameText("Armour:", vec3(-0.98, 0.7, 0), colour, kerning);
	armourTitle.setScale(vec3(1.f));

	armourTex.InitializeGameText("000", vec3(-0.73, 0.7, 0), colour, kerning);
	armourTex.setScale(vec3(1.2f));

	//velocity
	velocityTitle.InitializeGameText("mph", vec3(0.86, -0.85, 0), colour, kerning);

	velocityTex.InitializeGameText("00", vec3(0.62, -0.95, 0), colour, kerning);
	velocityTex.setScale(vec3(3.f));

	//border for health and armour
	topLeftBkgrd.GenerateSquareVertices(0.3, 0.2, vec3(0.6, 0.5, 1));
	topLeftBkgrd.setPosition(vec3(-0.8, 0.8, 0));
	topLeftBkgrd.setTransparency(0.4f);
	topLeftBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	topLeftBorder.GenerateBorder(0.3, 0.2, 0.1, vec3(0, 0, 0), vec3(-0.8, 0.8, 0));
	topLeftBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	//topLeftBorder.transparency = 0.5f;

	//border for velocity
	botRightBkgrd.GenerateSquareVertices(0.2, 0.2, vec3(0.6, 0.5, 1));
	botRightBkgrd.setPosition(vec3(0.8, -0.8, 0));
	botRightBkgrd.setTransparency(0.4f);
	botRightBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	botRightBorder.GenerateBorder(0.2, 0.2, 0.1, colour, vec3(0.8, -0.8, 0));
	botRightBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	//botRightBorder.transparency = 0.5f;

	//initialize radar
	radarPoints.GenerateVertices(positions, colour, &vector<vec2>(0));
	radarPoints.setPosition(vec3(0.8, 0.8, 0));
	radarPoints.setScale(vec3(0.1f, 0.15f, 1));
	//radarPoints.setTransparency(0.4f);
	radarPoints.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	//arena map background
	if (!radarBkgrd.initTexture(arenaFilename, GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}

	radarBkgrd.GenerateSquareVertices(0.2, 0.2, vec3(0.6, 0.5, 1));
	radarBkgrd.setPosition(vec3(0.8, 0.8, 0));
	radarBkgrd.setTransparency(0.4f);
	radarBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	radarBorder.GenerateBorder(0.2, 0.2, 0.1, vec3(0, 0, 0), vec3(0.8, 0.8, 0));
	radarBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	//screen border
	screenBorder.GenerateBorder(1, 1, 0.01, vec3(0, 0, 0), vec3(0, 0, 0));
	screenBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	//arena map background
	if (!dpadTexture.initTexture("textures/XboxControllerDPad.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}
	dpadTexture.GenerateSquareVertices(0.15, 0.18, vec3(0.6, 0.5, 0.1));
	dpadTexture.setPosition(vec3(-0.83,-0.8,0));
	dpadTexture.setTransparency(0.4f);
	dpadTexture.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	dpadBorder.GenerateBorder(0.17, 0.2, 0.1, vec3(0), vec3(-0.83,-0.8,0));
	dpadBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!weaponUpD.initTexture("textures/itemImgs/bearTrap.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}
	weaponUpD.setMixFlag(1);
	weaponUpD.GenerateSquareVertices(0.06, 0.07, vec3(0,0,1));
	weaponUpD.setPosition(vec3(-0.83, -0.66, 0));
	weaponUpD.setTransparency(0.8f);
	weaponUpD.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!weaponLeftD.initTexture("textures/itemImgs/bearTrap.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}

	weaponLeftD.setMixFlag(1);
	weaponLeftD.GenerateSquareVertices(0.06, 0.07, vec3(1, 1, 0));
	weaponLeftD.setPosition(vec3(-0.93, -0.79, 0));
	weaponLeftD.setTransparency(0.8f);
	weaponLeftD.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

//Updates radar point values (i.e., the cars)
///TODO: Fix scale of position points in comparison to map, add colours?
void GameHud::UpdateRadar(const vector<vec3> *positions, const vector<vec3> *colours) {
	vector<vec3> newPositions;
	vec3 newVec;
	for (int i = 0; i < positions->size(); i++) {
		newVec = positions->at(i) * 0.003f;

		//Since 2D and y vec is useless for radar map, want x and z coordinates instead
		newPositions.push_back(vec3(-newVec.x, newVec.z, 0));

	}
	radarPoints.UpdateVertices(&newPositions);
	radarPoints.updateColourBuffer2(*colours);
}

//render hud
void GameHud::Render(const string &health, const string &armour, const string &velocity, const vector<vec3>*positions, const vec3 &colour, const vector<vec3> *colours, const bool &canLayTrap) {
	//update values
	int kerning = 30;
	healthTex.UpdateGameText(health, kerning);
	armourTex.UpdateGameText(armour, kerning);
	velocityTex.UpdateGameText(velocity, kerning);
	UpdateRadar(positions, colours);

	//render widgets.  Render in order widget -> border ->background in order to show properly
	healthTitle.Render(GL_TRIANGLES, colour);
	armourTitle.Render(GL_TRIANGLES, colour);
	healthTex.Render(GL_TRIANGLES, colour);
	armourTex.Render(GL_TRIANGLES, colour);
	topLeftBorder.Render(GL_TRIANGLES, colour);
	topLeftBkgrd.Render(GL_TRIANGLE_STRIP, topLeftBkgrd.getColour());

	velocityTex.Render(GL_TRIANGLES, colour);
	velocityTitle.Render(GL_TRIANGLES, colour);
	botRightBorder.Render(GL_TRIANGLES, colour);
	botRightBkgrd.Render(GL_TRIANGLE_STRIP, botRightBkgrd.getColour());

	radarBorder.Render(GL_TRIANGLES, colour);
	radarPoints.Render(GL_POINTS, colour);
	radarBkgrd.Render(GL_TRIANGLE_STRIP, radarBkgrd.getColour());

	dpadBorder.Render(GL_TRIANGLES, colour);
	if (canLayTrap) {
		weaponUpD.setMixAmount(0.5);
		weaponUpD.Render(GL_TRIANGLE_STRIP, weaponUpD.getColour());
		weaponLeftD.setMixAmount(0.5);
		weaponLeftD.Render(GL_TRIANGLE_STRIP, weaponLeftD.getColour());
	}
	else {
		weaponUpD.setMixAmount(1.f);
		weaponUpD.Render(GL_TRIANGLE_STRIP, vec3(0));
		weaponLeftD.setMixAmount(1.f);
		weaponLeftD.Render(GL_TRIANGLE_STRIP, vec3(0));
	}
	dpadTexture.Render(GL_TRIANGLE_STRIP, dpadTexture.getColour());

	screenBorder.Render(GL_TRIANGLES, colour);
}

void GameHud::RenderEndGame(const int &menuIndex, const vec3 &colour) {
	float multiplyColour = 0.3f;

	if (menuIndex == 0) {
		quitText.setColour(colour * multiplyColour);
		restartText.setColour(colour);
	}
	else {
		quitText.setColour(colour);
		restartText.setColour(colour*multiplyColour);
	}

	quitText.Render(GL_TRIANGLES, quitText.getColour());
	restartText.Render(GL_TRIANGLES, restartText.getColour());
	firstPlace.Render(GL_TRIANGLES, firstPlace.getColour());
	secondPlace.Render(GL_TRIANGLES, secondPlace.getColour());
	thirdPlace.Render(GL_TRIANGLES, thirdPlace.getColour());
	fourthPlace.Render(GL_TRIANGLES, fourthPlace.getColour());
	endGameOverlay.Render(GL_TRIANGLE_STRIP, endGameOverlay.getColour());
}
void GameHud::RenderMenu(const int &menuIndex, const vec3 &colour) {
	float multiplyColour = 0.3f;
	if (menuIndex == 0) {
		restartText.setColour(colour*multiplyColour);
		quitText.setColour(colour * multiplyColour);
		resumeText.setColour(colour);
	}
	else if (menuIndex == 1) {
		quitText.setColour(colour * multiplyColour);
		restartText.setColour(colour);
		resumeText.setColour(colour * multiplyColour);
	}
	else {
		quitText.setColour(colour);
		restartText.setColour(colour*multiplyColour);
		resumeText.setColour(colour*multiplyColour);
	}
	quitText.Render(GL_TRIANGLES, quitText.getColour());
	resumeText.Render(GL_TRIANGLES, resumeText.getColour());
	restartText.Render(GL_TRIANGLES, restartText.getColour());
	pauseBorder.Render(GL_TRIANGLES, colour);
	pauseBox.Render(GL_TRIANGLE_STRIP, pauseBox.getColour());
	pauseBkgrd.Render(GL_TRIANGLE_STRIP, vec3(0));// vec3(1.f - colour) * 0.3f);
}