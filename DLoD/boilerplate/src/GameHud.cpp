#include "Game\GameHud.h"

//Game Hud class.

GameHud::GameHud()
{
}


GameHud::~GameHud()
{
}


void GameHud::InitializeMenu(const vec3 &colour) {
	resumeText.InitializeGameText("RESUME", vec3(-0.27, 0.0, 0), colour, 30);
	resumeText.setScale(vec3(2));
	restartText.InitializeGameText("RESTART", vec3(-0.3, -0.2, 0), colour, 30);
	restartText.setScale(vec3(2));
	quitText.InitializeGameText("QUIT", vec3(-0.2, -0.4, 0), colour, 30);
	quitText.setScale(vec3(2));

	//arena map background
	if (!pauseBkgrd.initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
		cout << "Failed to init arena map." << endl;
	}

	pauseBkgrd.GenerateSquareVertices(1, 1, vec3(0.6, 0.5, 1));
	pauseBkgrd.setMixFlag(1);
	pauseBkgrd.setMixAmount(0.5);
	pauseBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	
	//border for health and armour
	pauseBox.GenerateSquareVertices(0.25, 0.32, vec3(0.6, 0.7, 1));
	pauseBox.setPosition(vec3(-0.055, -0.07, 0));
	pauseBox.setTransparency(0.2f);
	pauseBox.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	pauseBorder.GenerateBorder(0.25, 0.32, 0.01, vec3(0, 0, 0), vec3(-0.055, -0.07, 0));
	pauseBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

}
//initialize hud
void GameHud::InitializeHud(const vec3 &colour, const vector<vec3> *positions, const string &arenaFilename) {
	cout << "Initializing hud. " << endl;
	//health
	healthTitle.InitializeGameText("Health:", vec3(-0.98, 0.88, 0), colour, 30);
	healthTitle.setScale(vec3(0.8));

	healthTex.InitializeGameText("000", vec3(-0.73, 0.88, 0), colour, 30);
	healthTex.setScale(vec3(0.8));

	//armour
	armourTitle.InitializeGameText("Armour:", vec3(-0.98, 0.78, 0), colour, 30);
	armourTitle.setScale(vec3(0.8));

	armourTex.InitializeGameText("000", vec3(-0.73, 0.78, 0), colour, 30);
	armourTex.setScale(vec3(0.8));

	//velocity
	velocityTitle.InitializeGameText("mph", vec3(0.86, -0.85, 0), colour, 30);

	velocityTex.InitializeGameText("00", vec3(0.62, -0.95, 0), colour, 30);
	velocityTex.setScale(vec3(3.f));

	//border for health and armour
	topLeftBkgrd.GenerateSquareVertices(0.2, 0.15, vec3(0.6, 0.5, 1));
	topLeftBkgrd.setPosition(vec3(-0.8, 0.85, 0));
	topLeftBkgrd.setTransparency(0.4f);
	topLeftBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	topLeftBorder.GenerateBorder(0.2, 0.15, 0.1, vec3(0, 0, 0), vec3(-0.8, 0.85, 0));
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
	radarPoints.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	//arena map background
	if (!radarBkgrd.initTexture(arenaFilename, GL_TEXTURE_2D)) {
		cout << "Failed to init arena map." << endl;
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
		cout << "Failed to init arena map." << endl;
	}
	dpadTexture.isRedTransparent = 1;
	dpadTexture.GenerateSquareVertices(0.15, 0.18, vec3(0.6, 0.5, 0.1));
	dpadTexture.setPosition(vec3(-0.83,-0.8,0));
	dpadTexture.setTransparency(0.4f);
	dpadTexture.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	dpadBorder.GenerateBorder(0.17, 0.2, 0.1, vec3(0), vec3(-0.83,-0.8,0));
	dpadBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!weaponUpD.initTexture("textures/itemImgs/bearTrap.png", GL_TEXTURE_2D)) {
		cout << "Failed to init arena map." << endl;
	}
	weaponUpD.isRedTransparent = 1;
	weaponUpD.setMixFlag(1);
	weaponUpD.GenerateSquareVertices(0.06, 0.07, vec3(1,1,0));
	weaponUpD.setPosition(vec3(-0.83, -0.66, 0));
	weaponUpD.setTransparency(0.8f);
	weaponUpD.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!weaponLeftD.initTexture("textures/itemImgs/bearTrap.png", GL_TEXTURE_2D)) {
		cout << "Failed to init arena map." << endl;
	}

	weaponLeftD.isRedTransparent = 1;
	weaponLeftD.setMixFlag(1);
	weaponLeftD.GenerateSquareVertices(0.06, 0.07, vec3(0, 0, 1));
	weaponLeftD.setPosition(vec3(-0.93, -0.79, 0));
	weaponLeftD.setTransparency(0.8f);
	weaponLeftD.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

//Updates radar point values (i.e., the cars)
///TODO: Fix scale of position points in comparison to map, add colours?
void GameHud::UpdateRadar(const vector<vec3> *positions) {
	vector<vec3> newPositions;
	vec3 newVec;
	for (int i = 0; i < positions->size(); i++) {
		newVec = radarPoints.Normalize(positions->at(i));

		//Since 2D and y vec is useless for radar map, want x and z coordinates instead
		///TODO: Fix radar scaling and stuff
		//newPositions.push_back(vec3(-newVec.x/newVec.y, newVec.z/newVec.y,0) * 0.003f);
		newPositions.push_back(vec3(-newVec.x, newVec.z, 0)*0.1f);

	}
	radarPoints.UpdateVertices(&newPositions);
}

//render hud
void GameHud::Render(const string &health, const string &armour, const string &velocity, const vector<vec3>*positions, const vec3 &colour, const bool &canLayTrap) {
	//update values
	healthTex.UpdateGameText(health);
	armourTex.UpdateGameText(armour);
	velocityTex.UpdateGameText(velocity);
	UpdateRadar(positions);

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

void GameHud::RenderMenu(const int &menuIndex, const vec3 &colour) {
	if (menuIndex == 0) {
		restartText.setColour(colour*0.3f);
		quitText.setColour(colour * 0.3f);
		resumeText.setColour(colour);
	}
	else if (menuIndex == 1) {
		quitText.setColour(colour * 0.3f);
		restartText.setColour(colour);
		resumeText.setColour(colour * 0.3f);
	}
	else {
		quitText.setColour(colour);
		restartText.setColour(colour*0.3f);
		resumeText.setColour(colour*0.3f);
	}
	quitText.Render(GL_TRIANGLES, quitText.getColour());
	resumeText.Render(GL_TRIANGLES, resumeText.getColour());
	restartText.Render(GL_TRIANGLES, restartText.getColour());
	pauseBorder.Render(GL_TRIANGLES, colour);
	pauseBox.Render(GL_TRIANGLE_STRIP, pauseBox.getColour());
	pauseBkgrd.Render(GL_TRIANGLE_STRIP, vec3(1.f-colour) * 0.3f);
}

void GameHud::Destroy() {
	//kill all the ScreenOverlays
	healthTex.Destroy();
	armourTex.Destroy();
	healthTitle.Destroy();
	armourTitle.Destroy();
	topLeftBkgrd.Destroy(); 
	topLeftBorder.Destroy();
	botRightBorder.Destroy(); 
	botRightBkgrd.Destroy();
	velocityTitle.Destroy(); 
	velocityTex.Destroy();
	radarBkgrd.Destroy(); 
	radarBorder.Destroy();
	radarPoints.Destroy(); 
	screenBorder.Destroy();
	resumeText.Destroy(); 
	quitText.Destroy();
	pauseBkgrd.Destroy(); 
	pauseBox.Destroy(); 
	pauseBorder.Destroy();
	restartText.Destroy();
	dpadBorder.Destroy();
	dpadTexture.Destroy();
	weaponUpD.Destroy();
	weaponLeftD.Destroy();
}