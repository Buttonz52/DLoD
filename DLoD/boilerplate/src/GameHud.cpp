#include "..\headers\Game\GameHud.h"

//Game Hud class.

GameHud::GameHud()
{
}


GameHud::~GameHud()
{
}

//Updates radar point values (i.e., the cars)
///TODO: Fix scale of position points in comparison to map, add colours?
void GameHud::UpdateRadar(const vector<vec3> *positions) {
	vector<vec3> newPositions;
	vec3 newVec;
	for (int i = 0; i < positions->size(); i++) {
		newVec = radarPoints.Normalize(positions->at(i));

		//Since 2D and y vec is useless for radar map, want x and z coordinates instead
		newPositions.push_back(vec3(-newVec.x, newVec.z,0)*0.1f);
	}
	radarPoints.UpdateVertices(&newPositions);
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
	topLeftBkgrd.transparency = 0.4f;
	topLeftBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	topLeftBorder.GenerateBorder(0.2, 0.15, 0.1, vec3(0, 0, 0), vec3(-0.8, 0.85, 0));
	topLeftBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	//topLeftBorder.transparency = 0.5f;

	//border for velocity
	botRightBkgrd.GenerateSquareVertices(0.2, 0.2, vec3(0.6, 0.5, 1));
	botRightBkgrd.setPosition(vec3(0.8, -0.8, 0));
	botRightBkgrd.transparency = 0.4f;
	botRightBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	botRightBorder.GenerateBorder(0.2, 0.2, 0.1, vec3(0, 0, 0), vec3(0.8, -0.8, 0));
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
	radarBkgrd.transparency = 0.4f;
	radarBkgrd.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	radarBorder.GenerateBorder(0.2, 0.2, 0.1, vec3(0, 0, 0), vec3(0.8, 0.8, 0));
	radarBorder.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

//render hud
void GameHud::Render(const string &health, const string &armour, const string &velocity, const vector<vec3>*positions) {
	//update values
	healthTex.UpdateGameText(health);
	armourTex.UpdateGameText(armour);
	velocityTex.UpdateGameText(velocity);
	UpdateRadar(positions);

	//render widgets.  Render in order widget -> border ->background in order to show properly
	healthTitle.Render(GL_TRIANGLES);
	armourTitle.Render(GL_TRIANGLES);
	healthTex.Render(GL_TRIANGLES);
	armourTex.Render(GL_TRIANGLES);
	topLeftBorder.Render(GL_TRIANGLES);
	topLeftBkgrd.Render(GL_TRIANGLE_STRIP);

	velocityTex.Render(GL_TRIANGLES);
	velocityTitle.Render(GL_TRIANGLES);
	botRightBorder.Render(GL_TRIANGLES);
	botRightBkgrd.Render(GL_TRIANGLE_STRIP);

	radarBorder.Render(GL_TRIANGLES);
	radarPoints.Render(GL_POINTS);
	radarBkgrd.Render(GL_TRIANGLE_STRIP);

}