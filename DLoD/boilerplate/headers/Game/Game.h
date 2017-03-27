#ifndef GAME_H
#define GAME_H


#include "GEO/Player/AI.h"
#include "GEO/player/Human.h"
#include "Physics/PhysXMain.h"
#include "Game\ScreenOverlay.h"
#include "Game\Audio.h"
#include "Game\Timer.h"
#include "GEO/Shadow.h"
#include "Game/GameHud.h"
#include "Game/Billboard.h"

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
  bool pause, restart;
  Billboard billboard;
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
  void goToGamePausedState();
  void initVehicle(Vehicle * v, int type);
  void UpdateHudInfo(Player * player, mat4 & projectionMatrix, mat4 & viewMatrix, string & healthStr, string & armourStr, string & velocityStr, vec3 & vColour, bool & canLayTrap);
  void UpdateHudInfoEmpty(const vector <Player*> players, const int &i, mat4 &projectionMatrix, mat4 &viewMatrix, Camera &winningCam, Camera &overheadCam, string &healthStr, string &armourStr, string &velocityStr, vec3 &vColour);
  void initItem(Item * item);

  void gameLoop();

  //void InitializeGameText(ScreenOverlay *fontTex, const string &text, const vec3 &position, const vec3&colour, int kerning);

  //void UpdateGameText(ScreenOverlay * fontText, const string &text);

  //void GenerateTextUVs(vector<vec2>& uvs, const char &c);

  //Mix_Chunk *winSFX = Mix_LoadWAV("sfx/win.wav");
  //Mix_Chunk *loseSFX = Mix_LoadWAV("sfx/lose.wav");

public:

  Game(GLFWwindow *w, Audio audio, const string &skyboxFilepath, const string &arenaFilepath, const string &starObjFilename, const string &arenaMapFile, const vector<int> *humanVehicleChoice, const int numPlayers);

  bool start();
};

#endif
