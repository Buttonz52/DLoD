#ifndef GAME_H
#define GAME_H


#include "../GEO/Player/AI.h"
#include "../GEO/player/Human.h"
#include "../Physics/PhysXMain.h"
#include "..\headers\Game\ScreenOverlay.h"
#include "..\headers\Game\Audio.h"
#include "..\headers\Game\Timer.h"

#include "../Game/GameHud.h"

class Game
{
private:
  GEO* skybox;
  GEO* arena;
  Camera overheadCam, winningCam;	//camera of entire arena, camera of current winning car
  vector<Player*> players;
  vector<GEO*> physXObjects;
  GLFWwindow *window;
  string arenaTexFilename;
  string arenaMap;
  int width, height;
  int numPlayerScreens, pauseIdentifier, menuIndex;
  ScreenOverlay pauseText;
  Timer timer;
  bool pause;

  // vector of actors 
  vector<pair<Player*, int>> itemsToAdd;

  PhysXMain physX;
  //GLFWwindow *window;
  Audio audio;

  GameHud gameHud;

  vec3 lightSource = vec3(0.f, 100.f, 0.f);

  bool gameOver = false;

  void initSkyBox(const string &pathname);
  GEO * initArena(const string &texfilename, const string &objfilename);
  void ResizeViewport(const int index, const int numHumans, const int width, const int height);
  void initVehicle(Vehicle * v, int type);

  void initItem(Item * item);

  void gameLoop();

  //void InitializeGameText(ScreenOverlay *fontTex, const string &text, const vec3 &position, const vec3&colour, int kerning);

  //void UpdateGameText(ScreenOverlay * fontText, const string &text);

  //void GenerateTextUVs(vector<vec2>& uvs, const char &c);

  Mix_Chunk *winSFX = Mix_LoadWAV("sfx/win.wav");
  Mix_Chunk *loseSFX = Mix_LoadWAV("sfx/lose.wav");

public:

  Game(GLFWwindow *w, Audio audio, const string &skyboxFilepath, const string &arenaFilepath, const string &arenaMapFile, const vector<int> *humanVehicleChoice, const int numPlayers);

  void start();
};

#endif
