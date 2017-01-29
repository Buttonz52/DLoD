#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>

class Audio {
public: 
	Audio();
	~Audio();

	bool InitMusic();

	void CleanUp();

	bool PlayMusic();

	void PlaySfx(Mix_Chunk *sfx);

	void PauseMusic();

	void ResumeMusic();

	void PausePlay();

	Mix_Chunk *horn = NULL;

private:
	bool on;

	Mix_Music *music_file = NULL;
}
;
#endif