#ifndef GAME_H
#define GAME_H


#include "../GEO/Player/AI.h"
#include "../GEO/player/Human.h"
#include "../Physics/PhysXMain.h"
#include "..\headers\Game\ScreenOverlay.h"
#include "..\headers\Game\Audio.h"
#include "..\headers\Game\Timer.h"

class Game
{
private:
  GEO* skybox;
  GEO* arena;
  vector<Player*> players;
  vector<GEO*> physXObjects;
  string arenaTexFilename;
  string arenaBumpmap;

  Timer timer;

  // vector of actors 
  vector<pair<Player*, int>> itemsToAdd;

  PhysXMain physX;
  GLFWwindow *window;
  Audio audio;

  ScreenOverlay logo, healthTex, armourTex, healthTitle, armourTitle;

  vec3 lightSource = vec3(0.f, 100.f, 0.f);

  bool gameOver = false;

  void initSkyBox(const string &pathname);
  GEO * initArena(const string &texfilename, const string &objfilename);
  void initVehicle(Vehicle * v);

  void initItem(Item * item);

  void gameLoop();

  //void InitializeGameText(ScreenOverlay *fontTex, const string &text, const vec3 &position, const vec3&colour, int kerning);

  //void UpdateGameText(ScreenOverlay * fontText, const string &text);

  //void GenerateTextUVs(vector<vec2>& uvs, const char &c);

  Mix_Chunk *winSFX = Mix_LoadWAV("sfx/win.wav");
  Mix_Chunk *loseSFX = Mix_LoadWAV("sfx/lose.wav");

public:

  Game(GLFWwindow*, Audio audio, const string &skyboxFilepath, const string &arenaFilepath);

  void start();
};

#endif
