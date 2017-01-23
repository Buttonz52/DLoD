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

	void PauseMusic();

private:
	Mix_Music *music_file = NULL;
}
;
#endif