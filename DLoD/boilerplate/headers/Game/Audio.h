#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;
enum channel {
	select, throwTrap, hitItem, menu, other
};
class Audio {
public: 
	Audio();
	~Audio();

	bool InitMusic(const char *filename);
	void FreeMusic();
	void CleanUp();
	bool PlayMusic();
	void PauseMusic();
	void ChangeMusicVolume(const int &volume);
	void PlaySfx(const string &sfx_string, const int & volume, const channel & channel);
	//void PlaySfx(Mix_Chunk *sfx, const int &volume, const int &channel);
	void ResumeMusic();
	void PausePlay();
	bool Init();
	bool InitAllSounds();

private:
	bool on;
	map<string, Mix_Chunk*> audioMap;
	Mix_Music *music_file = NULL;

}
;
#endif