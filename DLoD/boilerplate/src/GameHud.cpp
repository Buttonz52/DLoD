#include "Game\GameHud.h"

//Game Hud class.

GameHud::GameHud()
{
}

GameHud::GameHud(const gameMode &version) {
	mode = version;

}

GameHud::~GameHud()
{
}

void GameHud::setMode(const gameMode &version) {
	mode = version;
}

//initialize hud
void GameHud::InitializeHud(const vec3 &colour, const vector<vec3> *positions, const string &arenaFilename) {
	int kerning = 30;
	//health
	healthTitle.InitializeGameText("Health:", vec3(-0.98, 0.8, 0), colour, kerning);
	healthTitle.SetScale(vec3(1.f));

	healthTex.InitializeGameText("000", vec3(-0.73, 0.8, 0), colour, kerning);
	healthTex.SetScale(vec3(1.2f));

	//armour
	armourTitle.InitializeGameText("Armour:", vec3(-0.98, 0.7, 0), colour, kerning);
	armourTitle.SetScale(vec3(1.f));

	armourTex.InitializeGameText("000", vec3(-0.73, 0.7, 0), colour, kerning);
	armourTex.SetScale(vec3(1.2f));

	if (mode == time) {
		timeTex.InitializeGameText("00", vec3(-0.2, 0.7, 0), colour, kerning);
		timeTex.SetScale(vec3(3.f));
	}

	//velocity
	velocityTitle.InitializeGameText("mph", vec3(0.86, -0.85, 0), colour, kerning);

	velocityTex.InitializeGameText("00", vec3(0.62, -0.95, 0), colour, kerning);
	velocityTex.SetScale(vec3(3.f));

	//border for health and armour
	topLeftBkgrd.GenerateSquareVertices(0.3, 0.2, vec3(0.6, 0.5, 1));
	topLeftBkgrd.SetPosition(vec3(-0.8, 0.8, 0));
	topLeftBkgrd.SetTransparency(0.4f);
	topLeftBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	topLeftBorder.GenerateBorder(0.3, 0.2, 0.1, vec3(0, 0, 0), vec3(-0.8, 0.8, 0));
	topLeftBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	//topLeftBorder.transparency = 0.5f;

	//border for velocity
	botRightBkgrd.GenerateSquareVertices(0.2, 0.2, vec3(0.6, 0.5, 1));
	botRightBkgrd.SetPosition(vec3(0.8, -0.8, 0));
	botRightBkgrd.SetTransparency(0.4f);
	botRightBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	botRightBorder.GenerateBorder(0.2, 0.2, 0.1, colour, vec3(0.8, -0.8, 0));
	botRightBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	//botRightBorder.transparency = 0.5f;

	//initialize radar
	radarPoints.GenerateVertices(positions, colour, &vector<vec2>(0));
	radarPoints.SetPosition(vec3(0.8, 0.8, 0));
	radarPoints.SetScale(vec3(0.1f, 0.15f, 1));
	//radarPoints.SetTransparency(0.4f);
	radarPoints.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	//arena map background
	if (!radarBkgrd.InitTexture(arenaFilename, GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}

	radarBkgrd.GenerateSquareVertices(0.2f, 0.2f, vec3(0.6f, 0.5f, 1.f));
	radarBkgrd.SetPosition(vec3(0.8f, 0.8f, 0.f));
	radarBkgrd.SetTransparency(0.4f);
	radarBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	radarBorder.GenerateBorder(0.2f, 0.2f, 0.1f, vec3(0.f, 0.f, 0.f), vec3(0.8f, 0.8f, 0.f));
	radarBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	//screen border
	screenBorder.GenerateBorder(1, 1, 0.01, vec3(0, 0, 0), vec3(0, 0, 0));
	screenBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	//arena map background
	if (!dpadTexture.InitTexture("textures/XboxControllerDPad.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}
	dpadTexture.GenerateSquareVertices(0.15, 0.18, vec3(0.6, 0.5, 0.1));
	dpadTexture.SetPosition(vec3(-0.83,-0.8,0));
	dpadTexture.SetTransparency(0.4f);
	dpadTexture.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	dpadBorder.GenerateBorder(0.17, 0.2, 0.1, vec3(0), vec3(-0.83,-0.8,0));
	dpadBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!weaponUpD.InitTexture("textures/itemImgs/bearTrap.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}
	weaponUpD.SetMixFlag(1);
	weaponUpD.GenerateSquareVertices(0.06, 0.07, vec3(0,0,1));
	weaponUpD.SetPosition(vec3(-0.83, -0.66, 0));
	weaponUpD.SetTransparency(0.8f);
	weaponUpD.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!weaponLeftD.InitTexture("textures/itemImgs/bearTrap.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init arena map." << endl;
	}

	weaponLeftD.SetMixFlag(1);
	weaponLeftD.GenerateSquareVertices(0.06, 0.07, vec3(1, 1, 0));
	weaponLeftD.SetPosition(vec3(-0.93, -0.79, 0));
	weaponLeftD.SetTransparency(0.8f);
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
	radarPoints.UpdateVertexBuffer(&newPositions);
	radarPoints.UpdateColourBuffer2(*colours);
}

//render hud
void GameHud::Render(vector<string> &hudStrings, const vector<vec3>*positions, const vec3 &colour, const vector<vec3> *colours, const bool &canLayTrap) {
	//update values
	int kerning = 30;
	healthTex.UpdateGameText(hudStrings[0], kerning);
	armourTex.UpdateGameText(hudStrings[1], kerning);
	velocityTex.UpdateGameText(hudStrings[2], kerning);

	mode == time ? timeTex.UpdateGameText(hudStrings[3], kerning) : 0;
	UpdateRadar(positions, colours);

	//render widgets.  Render in order widget -> border ->background in order to show properly
	healthTitle.Render(GL_TRIANGLES, colour);
	armourTitle.Render(GL_TRIANGLES, colour);
	healthTex.Render(GL_TRIANGLES, colour);
	armourTex.Render(GL_TRIANGLES, colour);
	mode == time ? timeTex.Render(GL_TRIANGLES, colour) : 0;
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
		weaponUpD.SetMixAmount(0.5);
		weaponUpD.Render(GL_TRIANGLE_STRIP, weaponUpD.getColour());
		weaponLeftD.SetMixAmount(0.5);
		weaponLeftD.Render(GL_TRIANGLE_STRIP, weaponLeftD.getColour());
	}
	else {
		weaponUpD.SetMixAmount(1.f);
		weaponUpD.Render(GL_TRIANGLE_STRIP, vec3(0));
		weaponLeftD.SetMixAmount(1.f);
		weaponLeftD.Render(GL_TRIANGLE_STRIP, vec3(0));
	}
	dpadTexture.Render(GL_TRIANGLE_STRIP, dpadTexture.getColour());

	screenBorder.Render(GL_TRIANGLES, colour);
}