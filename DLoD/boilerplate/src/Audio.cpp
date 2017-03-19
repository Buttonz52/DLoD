#include "Game/Audio.h"

Audio::Audio()
{
}

Audio::~Audio()
{
}

bool Audio::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}
	//Open the SDL_Mixer to a frequency of 22050, 2 channels (stereo), and
	//the size of chunks when playing sound.
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		return false;
	}
	return true;
}
bool Audio::InitMusic(const char *filename) {
	//Init();
	//Load the music file.  If there's nothing in the file, return false.
	music_file = Mix_LoadMUS(filename);
	horn = Mix_LoadWAV("sfx/horn.wav");
	chicken = Mix_LoadWAV("sfx/startNoise.wav");

	if (music_file == NULL || horn == NULL|| chicken == NULL) {
		return false;
	}
	//Return true if everything is good.
	return true;
}

void Audio::FreeMusic() {
	Mix_FreeMusic(music_file);
	//Mix_CloseAudio();
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

void Audio::ChangeMusicVolume(const int &volume) {
	Mix_VolumeMusic(volume);
}
void Audio::PlaySfx(Mix_Chunk *sfx, const int &volume, const int &channel) {
	Mix_VolumeChunk(sfx, volume);
	Mix_PlayChannel(channel, sfx, 0);
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