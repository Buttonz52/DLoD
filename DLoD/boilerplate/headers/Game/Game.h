#ifndef GAME_H
#define GAME_H


#include "../GEO/Player/AI.h"
#include "../GEO/player/Human.h"
#include "../Physics/PhysXMain.h"
#include "..\headers\Game\ScreenOverlay.h"
#include "..\headers\Game\Audio.h"

class Game
{
private:
  GEO* skybox;
  vector<Player*> players;
  vector<GEO*> physXObjects;
  PhysXMain physX;
  GLFWwindow *window;
  Audio audio;

  ScreenOverlay logo, fontTex;

  vec3 lightSource = vec3(0.f, 100.f, 0.f);

  bool gameOver = false;

  void initSkyBox();
  void initVehicle(Vehicle * v);

  void gameLoop();

  void InitializeGameText(ScreenOverlay *fontTex, const string &text, const vec3 &position, const vec3&colour);

  void UpdateGameText(ScreenOverlay * fontText, const string &text);

  void GenerateTextUVs(vector<vec2>& uvs, const char &c);

  //string  = "sfx/win.wav";
  Mix_Chunk *winSFX = Mix_LoadWAV("sfx/win.wav");

public:

  Game(GLFWwindow*, Audio audio);

  void start();
};

#endif
