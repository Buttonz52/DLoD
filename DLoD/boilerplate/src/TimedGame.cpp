#include "Game\TimedGame.h"

TimedGame::~TimedGame()
{
}

TimedGame::TimedGame(GLFWwindow * w, Audio & audio, const string & skyboxFilepath, const string & arenaFilepath, const string & starObjFilename, const string & arenaMapFile, const vector<int> &humanVehicleChoice, const int & numPlayers, const vector<vec3>& spawnPoints, const vector<vec3>& itemSpawnPoints)
{
	timeOver = false;
	int duration_seconds = 60;	//how long game lasts for
	endGameTickNum = duration_seconds * 1000;	//amount of ticks to pass until game is over
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
	gameHud.setMode(GameHud::time);
	gameHud.InitializeHud(*players[0]->vehicle->getColour(), &positions, arenaMap);
}

bool TimedGame::start() {
	//start game timer
	if (!gameTimer.isStarted()) {
		gameTimer.start();
		currentTicks = gameTimer.getTicks();
	}
	switchCamText.SetScale(vec3(0.5f));
	switchCamText.InitializeGameText("Press <TAB> or <BACK> to change between cameras", vec3(-0.4, -0.8, 0), vec3(1, 1, 1), 30);

	// Enter the game Loop
	gameState->timer.start();
	for (ItemSpawner* s : gameState->itemSpawners)
		s->timer.start();
	gameLoop();

	// Clean up and Display the win screen

	///TODO: implmement for all screens -> lose/win for appropriate player
	ScreenOverlay endGameText;
	endGameText.SetScale(vec3(4.f));
	if (!gameOver) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//  for (int i = 0; i < numPlayerScreens; i++) {
		ResizeViewport(0, 1, width, height);
		endGameText.InitializeGameText("TIME OVER", vec3(-0.7, 0, 0), vec3(0), 30);
		endGameText.Render(GL_TRIANGLES, endGameText.getColour());
		//  }
	}
	else {
		for (int i = 0; i < numPlayerScreens; i++) {
			ResizeViewport(i, numPlayerScreens, width, height);
			if (players[i]->isDead()) {
				endGameText.InitializeGameText("LOSE!", vec3(-0.35, 0, 0), vec3(1, 0, 0), 30);
			}
			else {
				endGameText.InitializeGameText("WIN!", vec3(-0.3, 0, 0), vec3(0, 1, 0), 30);
			}
			endGameText.Render(GL_TRIANGLES, endGameText.getColour());
		}
	}
	glfwSwapBuffers(window);
	Sleep(1000);
	//	audio.PlaySfx(winSFX);
	//	audio.PlaySfx(loseSFX);

	bool pause = true;
	Human *h = dynamic_cast<Human*> (players[0]);

	vector <string> loserVectorNames;
	vector<vec3> loserVectorColours;
	vector<clock_t> ToDs;

	calculateRank(loserVectorNames, loserVectorColours, ToDs);
	goToEndGameState(loserVectorNames, loserVectorColours);

	delete skybox;
	delete arena;
	physX.cleanupPhysics(true);
	return restart;
}
void TimedGame::UpdateHudInfo(Player *player, mat4 & projectionMatrix, mat4 & viewMatrix, vector<string>& strings, vec3 & vColour, bool & canLayTrap)
{
	int numTicks = ((int)endGameTickNum - (int)currentTicks) / 1000;	//convert to seconds
	GameFactory::UpdateHudInfo(player, projectionMatrix, viewMatrix, strings, vColour, canLayTrap);
	if (numTicks < 10)
		strings[3] = string("0" + to_string(numTicks));
	else
		strings[3] = to_string(numTicks);
}

void TimedGame::UpdateHudInfoEmpty(const vector<Player*> players, const int & i, mat4 & projectionMatrix, mat4 & viewMatrix, Camera & winningCam, Camera & overheadCam, vector<string>& strings, vec3 & vColour, const int & camIndex)
{
	//cout << endGameTickNum - currentTicks << endl;
	int numTicks = ((int)endGameTickNum - (int)currentTicks) / 1000;	//convert to seconds
	GameFactory::UpdateHudInfoEmpty(players, i, projectionMatrix, viewMatrix, winningCam, overheadCam, strings, vColour, camIndex);
	if (numTicks < 10)
		strings[3] = string("0" + to_string(numTicks));
	else
		strings[3] = to_string(numTicks);
}


void TimedGame::gameLoop()
{
	vector<vec3> positions, radarColours;
	for (Player *p : players) {
		positions.push_back(p->vehicle->getModelMatrix()[3]);
		radarColours.push_back(*p->getColour());
	}
	//Initialize hud

	int frameCtr = 0;

	while (!glfwWindowShouldClose(window) && !gameOver && !restart && !timeOver)
	{
		//game paused
		while (pause && !glfwWindowShouldClose(window)) {
			goToGamePausedState();
		}
		//currentTicks = (int)gameTimer.getTicks();
		(currentTicks = (int) gameTimer.getTicks())> endGameTickNum ? timeOver = true : 0;	//check for time over
		// Add items to the scene
		vector<pair<Player*, int>>::iterator itr = itemsToAdd.begin();
		while (itr != itemsToAdd.end()) {
			if (itr->second < gameState->timer.getTicks()) {
				itr->first->ableToTrap = true;
				itr->first->layTrap = false;
				itr = itemsToAdd.erase(itr);
				break;
			}
			else {
				++itr;
			}
		}

		// remove the deleted geometry
		for (GEO* g : physX.deletedGeos)
		{
			skybox->removeChild(g);
			delete g;
		}

		physX.deletedGeos.clear();

		// Step Physx
		physX.stepPhysics(true, physXObjects);

		//clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//render screens
		//TODO: Make less.. ugly and long
		int viewports = numPlayerScreens;
		if (numPlayerScreens >= 3) {
			viewports = 4;
		}
		mat4 projectionMatrix, viewMatrix;
		const int hudStrings_size = 4;
		vector<string> hudStrings;
		for (int i = 0; i < hudStrings_size; i++) {
			hudStrings.push_back("00");
		}
		bool canLayTrap;
		vec3 vColour;
		for (int i = 0; i < viewports; i++) {
			//getviewport

			// Render
			//make sure rendering a player screen that 4
			//1) exists
			//2) is not dead
			//3) is defined as a human
			if (i < players.size() && !players[i]->isDead() && i < numPlayerScreens) {
				UpdateHudInfo(players[i], projectionMatrix, viewMatrix, hudStrings, vColour, canLayTrap);
			}
			//player is dead
			else {
				int camIndex = 0;
				Human* human = dynamic_cast<Human*> (players[i]);
				if (human != nullptr) {
					camIndex = human->camIndex;
				}
				UpdateHudInfoEmpty(players, i, projectionMatrix, viewMatrix, winningCam, overheadCam, hudStrings, vColour, camIndex);
			}

			//render normally
			//resize the viewport
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			ResizeViewport(i, numPlayerScreens, width, height);

			arena->Render(viewMatrix, projectionMatrix, lightSource);
			glDisable(GL_CULL_FACE);
			skybox->Render(viewMatrix, projectionMatrix, lightSource);

			if (players[i]->isDead())
				switchCamText.Render(GL_TRIANGLES, vColour);
			gameHud.Render(hudStrings, &positions, vColour, &radarColours, canLayTrap);
		}

		glfwSwapBuffers(window);

		positions.clear();
		radarColours.clear();
		// For all players get input
		for (Player* p : players)
		{
			//get radar points for radar map
			if (!p->isDead()) {
				positions.push_back(p->vehicle->getModelMatrix()[3]);
				radarColours.push_back(*p->getColour());
			}
			//kill player if out of bounds
			if (p->vehicle->getModelMatrix()[3].y < -10)
				p->vehicle->updateHealth(10000);

			AI* ai = dynamic_cast<AI*> (p);
			if (ai != nullptr && !ai->isDead())
			{
				ai->getInput(gameState);
				if (abs(ai->vehicle->physXVehicle->computeForwardSpeed()) > 20)
					ai->layTrap = true;
			}

			Human* human = dynamic_cast<Human*> (p);
			if (human != nullptr) {
				human->getInput(window, pause);
				if (human->pressedPause()) {
					menuIndex = 0;
					pauseIdentifier = human->identifier;
				}
			}

			p->playerCam->followVehicle(p->vehicle);

			if (p->layTrap && p->ableToTrap)
			{
				// create a new item at the appropriate location and add it to the items list
				mat4 M = p->vehicle->getModelMatrix();
				vec3 vPos = vec3(M[3]);
				vec3 dis = vec3(0, 8, 0);

				Item* item = new Item(p->trap, nullptr);
				mat4 m = mat4(1);
				m[3] = vec4(vPos + dis, 1);
				item->setModelMatrix(m);

				itemsToAdd.push_back(make_pair(p, gameState->timer.getTicks() + 5000));
				PxVec3 velocity = p->vehicle->physXVehicle->getRigidDynamicActor()->getLinearVelocity();

				initItem(item, velocity);

				p->layTrap = false;
				p->ableToTrap = false;
			}
		}

		for (ItemSpawner* spawner : gameState->itemSpawners)
		{
			if (spawner->timer.getTicks() > spawner->spawnTime && spawner->item == nullptr) {
				spawner->spawnItem(skybox);
				physX.initItem(spawner->item);
			}

		}

		// CHECK GAMEOVER
		int aliveCount = 0;
		for (Player* p : players)
			aliveCount += p->isDead() ? 0 : 1;

		//regenerate armour
		if (frameCtr % 60 * 8 == 0)
		{
			for (Player* p : players)
				p->vehicle->regenArmour();
		}
		frameCtr >= 60 * 8 ? frameCtr = 1 : frameCtr++;
		gameOver = aliveCount < 2;

		glfwPollEvents();
	}
}

void TimedGame::calculateRank(vector<string> &loserNames, vector<vec3> &loserColours, vector<clock_t> &ToDs) {
	cout << "calculating rank timed game" << endl;
	///TODO: make this into a virtual function
	//Adds players in rank based on when they died
	vector<Player*> alivePlayers;
	vector<string> aliveNames;
	vector<vec3> aliveColours;
	for (int i = 0; i < players.size(); i++) {
		std::stringstream fmt = formatRank(players[i], numPlayerScreens, i);

		//If not dead, then put in vector to deal with later
		if (!players[i]->isDead()) {
			alivePlayers.push_back(players[i]);

		}
		//Else, iterate over list and find proper location
		else {
			bool inList = false;
			for (int j = 0; j < ToDs.size(); j++) {
				if (!inList) {
					//died earlier than person
					if (players[i]->getTimeOfDeath() > ToDs[j] && ToDs[j] != 0) {
						loserNames.insert(loserNames.begin() + j, fmt.str());
						loserColours.insert(loserColours.begin() + j, *players[i]->getColour());
						ToDs.insert(ToDs.begin() + j, players[i]->getTimeOfDeath());
						inList = true;
					}
				}
			}
			//If not in list, then push to back of list (last place)
			if (!inList)
			{
				loserNames.push_back(fmt.str());
				loserColours.push_back(*players[i]->getColour());
				ToDs.push_back(players[i]->getTimeOfDeath());
			}
		}
	}
	//Players still alive.
	//need to check which player has most health/armour and rank them
	//based on that.
	//cout << "Initial: " << endl;
	//for (int i = 0; i < alivePlayers.size(); i++) {
	//	cout << alivePlayers[i]->vehicle->getHealth() << " " << alivePlayers[i]->vehicle->getArmour() << endl;
	//}

	int numAlive = alivePlayers.size();
	int j = 0;
	//sort most health/least health
	while (j <numAlive) {
		for (int i = j+1; i < numAlive; i++) {
			cout << i << " " <<j << endl;
			int compare = compareHealth(alivePlayers[j], alivePlayers[i]);
			if (compare ==1) {
				std::swap(alivePlayers[j], alivePlayers[i]);
			}
			//equal health, compare armour
			else if (compare == 0) {
				if (compareArmour(alivePlayers[j], alivePlayers[i]))
					std::swap(alivePlayers[j], alivePlayers[i]);
			}
		}
		j++;
	}

	//cout << "Final: " << endl;
	//for (int i = 0; i < numAlive; i++) {
	//	cout << alivePlayers[i]->vehicle->getHealth() << " " << alivePlayers[i]->vehicle->getArmour() << endl;
	//}
	for (int i = numAlive-1; i >=0; i--) {
		std::stringstream fmt = formatRank(alivePlayers[i], numPlayerScreens, i);
		loserNames.insert(loserNames.begin(), fmt.str());	//first place
		loserColours.insert(loserColours.begin(), *alivePlayers[i]->getColour());
		ToDs.insert(ToDs.begin(), 0);
	}
}

int TimedGame::compareHealth(const Player *p1, const Player *p2) {
	if (p1->vehicle->getHealth() < p2->vehicle->getHealth()) {
		return 1;
	}
	if (p1->vehicle->getHealth() == p2->vehicle->getHealth())
		return 0;
	return -1;
}

bool TimedGame::compareArmour(const Player *p1, const Player *p2) {
	if (p1->vehicle->getArmour() < p2->vehicle->getArmour()) {
		return true;
	}
	return false;
}

