#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>

class Audio {
public: 
	Audio();
	~Audio();

	Mix_Chunk *horn = NULL;

	bool InitMusic(const char *filename);
	void CleanUp();
	bool PlayMusic();
	void PauseMusic();
	void PlaySfx(Mix_Chunk *sfx);
	void ResumeMusic();
	void PausePlay();
	bool Init();



private:
	bool on;
	Mix_Music *music_file = NULL;
}
;
#endif