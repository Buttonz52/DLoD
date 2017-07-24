#include "Game\SuddenDeathGame.h"

SuddenDeathGame::SuddenDeathGame(GLFWwindow * w, Audio & audio, const string & skyboxFilepath, const string & arenaFilepath, const string & starObjFilename, const string & arenaMapFile, const vector<int> &humanVehicleChoice, const int & numPlayers, const vector<vec3>& spawnPoints, const vector<vec3>& itemSpawnPoints)
{
	pause = false, restart = false;
	menuIndex = 0;
	//set cameras so overtop and far-ish away from cars
	overheadCam.setAlt(-90);
	overheadCam.setRadius(450);

	winningCam.setAlt(-90);
	winningCam.setRadius(50);

	window = w;
	numPlayerScreens = numPlayers;
	glfwGetWindowSize(window, &width, &height);

	physX.init(8);

	srand(time(NULL));

	initSkyBox(skyboxFilepath);

	//create the starnode
	vector<vec3> astarVertices;
	GEO aStarGEO;
	aStarGEO.initMesh(starObjFilename);	//load vertices
	for (vec3 v : aStarGEO.getMesh().vertices)
	{
		v *= 30;
		astarVertices.push_back(v);
	}

	gameState = new GameState(astarVertices, itemSpawnPoints);

	arena = initArena(arenaTexFilename, arenaFilepath);
	arenaMap = arenaMapFile;

	for (int i = 0; i < numPlayers; i++) {
		Human* human = new Human(i, &audio);
		human->setNumCams(5);
		human->ChooseVehicle(humanVehicleChoice[i], &audio);
		human->vehicle->setPosition(spawnPoints[i]);
		human->vehicle->setEnvironmentMap(skybox->getTexture());
		initVehicle(human->vehicle, humanVehicleChoice[i]);
		skybox->children.push_back(human->vehicle);
		players.push_back(human);

		// Eventually this should be the only list of the players
		gameState->players.push_back(human);
	}

	//Adding a few AIs for the time being
	//random number generator to choose AI cars
	//create AIs so that there are 8 cars in the arena
	//NOTE: This gets pretty slow, might want to think about multi-threading or trying to run in release mode (but need to link those libraries)

	srand(time(NULL));
	int totalPlayers = 8;
	for (int i = numPlayers; i < totalPlayers; i++) {
		AI* ai = new AI(i);
		int aiRNGChoose = rand() % 3;
		ai->ChooseVehicle(aiRNGChoose, &audio);
		ai->vehicle->setPosition(spawnPoints[i]);
		ai->vehicle->setEnvironmentMap(skybox->getTexture());
		initVehicle(ai->vehicle, aiRNGChoose);
		skybox->children.push_back(ai->vehicle);
		players.push_back(ai);

		// Eventually this should be the only list of the players
		gameState->players.push_back(ai);
	}
	vector<vec3> positions;
	for (int i = 0; i < totalPlayers; i++) {
		positions.emplace_back();
	}
	gameHud.setMode(GameHud::sudden_death);
	gameHud.InitializeHud(*players[0]->vehicle->getColour(), &positions, arenaMap);
	gameHud.InitializeMenu(vec3(1, 0, 1));
}


SuddenDeathGame::~SuddenDeathGame()
{
}

void SuddenDeathGame::UpdateHudInfo(Player * player, mat4 & projectionMatrix, mat4 & viewMatrix, vector<string>& strings, vec3 & vColour, bool & canLayTrap)
{
	GameFactory::UpdateHudInfo(player, projectionMatrix, viewMatrix, strings, vColour, canLayTrap);
}

void SuddenDeathGame::UpdateHudInfoEmpty(const vector<Player*> players, const int & i, mat4 & projectionMatrix, mat4 & viewMatrix, Camera & winningCam, Camera & overheadCam, vector<string>& strings, vec3 & vColour, const int & camIndex)
{
	GameFactory::UpdateHudInfoEmpty(players, i, projectionMatrix, viewMatrix, winningCam, overheadCam, strings, vColour, camIndex);
}

void SuddenDeathGame::gameLoop() {
	GameFactory::gameLoop();	//default for now
}
bool SuddenDeathGame::start() {
	return GameFactory::start();
}