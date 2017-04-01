#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>

class Audio {
public: 
	Audio();
	~Audio();

	Mix_Chunk *horn = NULL;
	Mix_Chunk *chicken = NULL;

	bool InitMusic(const char *filename);
	void FreeMusic();
	void CleanUp();
	bool PlayMusic();
	void PauseMusic();
	void ChangeMusicVolume(const int &volume);
	void PlaySfx(Mix_Chunk *sfx, const int &volume, const int &channel);
	void ResumeMusic();
	void PausePlay();
	bool Init();

private:
	bool on;
	Mix_Music *music_file = NULL;
}
;
#endif