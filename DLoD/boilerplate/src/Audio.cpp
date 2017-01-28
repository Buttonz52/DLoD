#include "Game/Audio.h"



Audio::Audio()
{
	on;
}


Audio::~Audio()
{
}

bool Audio::InitMusic() {
	//Make sure everything initializes properly
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}
	//Open the SDL_Mixer to a frequency of 22050, 2 channels (stereo), and
	//the size of chunks when playing sound.
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		return false;
	}
	//Load the music file.  If there's nothing in the file, return false.
	music_file = Mix_LoadMUS("music/music.wav");
	if (music_file == NULL) {
		return false;
	}
	on = true;
	//Return true if everything is good.
	return true;
}

//Clean up resources.
void Audio::CleanUp()
{
	Mix_FreeMusic(music_file);
	Mix_CloseAudio();
	SDL_Quit();
}

bool Audio::PlayMusic() {
	if (Mix_PlayMusic(music_file, 9000) == -1) {
		return false;
	}
	return true;
}

void Audio::PauseMusic() {
	Mix_PauseMusic();
}

void Audio::ResumeMusic() {
	Mix_ResumeMusic();
}

void Audio::PausePlay() {
	if (on)
	{
		PauseMusic();
		on = false;
	}
	else
	{
		ResumeMusic();
		on = true;
	}
}