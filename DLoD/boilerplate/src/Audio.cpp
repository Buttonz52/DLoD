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
	//horn = Mix_LoadWAV("sfx/horn.wav");
	//chicken = Mix_LoadWAV("sfx/startNoise.wav");

	/* || horn == NULL|| chicken == NULL*/
	if (music_file == NULL) {
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
//void Audio::PlaySfx(Mix_Chunk *sfx, const int &volume, const int &channel) {
//	Mix_VolumeChunk(sfx, volume);
//	Mix_PlayChannel(channel, sfx, 0);
//}
void Audio::PlaySfx(const string &sfx_string, const int &volume, const channel &channel) {
	int chanInt = 0;
	switch (channel) {
	case select:
		chanInt = 0;
		break;
	case throwTrap:
		chanInt = 1;
		break;
	case hitItem:
		chanInt = 2;
		break;
	case menu:
		chanInt = 3;
		break;
	case other:
		chanInt = 4;
	default:
		chanInt = 0;
	}
	Mix_Chunk *sfx = audioMap[sfx_string];
	if (sfx != NULL) {
		Mix_VolumeChunk(audioMap[sfx_string], volume);
		Mix_PlayChannel(chanInt, audioMap[sfx_string], 0);
	}
	else
		cout << "Invalid audio file: " << sfx_string << endl;

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
bool Audio::InitAllSounds() {
	audioMap.insert(make_pair("horn", Mix_LoadWAV("sfx/horn.wav")));
	audioMap.insert(make_pair("chicken", Mix_LoadWAV("sfx/startNoise.wav")));
	audioMap.insert(make_pair("toggle", Mix_LoadWAV("sfx/menuToggle.wav")));
	audioMap.insert(make_pair("back",Mix_LoadWAV("sfx/back.wav")));
	audioMap.insert(make_pair("armourAndSelect",Mix_LoadWAV("sfx/getArmour.wav")));
	audioMap.insert(make_pair("getHealth",Mix_LoadWAV("sfx/getHealth.wav")));
	audioMap.insert(make_pair("hitDmg",Mix_LoadWAV("sfx/hitDmgTrap.wav")));
	audioMap.insert(make_pair("hitStun", Mix_LoadWAV("sfx/hitStunTrap.wav")));
	audioMap.insert(make_pair("throwDmg",Mix_LoadWAV("sfx/throwDmgTrap.wav")));
	audioMap.insert(make_pair("throwStun",Mix_LoadWAV("sfx/throwStunTrap.wav")));
	audioMap.insert(make_pair("crash", Mix_LoadWAV("sfx/carCrash.wav")));
	audioMap.insert(make_pair("explosion", Mix_LoadWAV("sfx/explosion.wav")));

	map<string, Mix_Chunk*>::iterator it;
	for (it = audioMap.begin(); it != audioMap.end(); it++) {
		if (it->second == NULL) {
			cout << "Error: Null audio file: " << it->first << endl;
			return false;
		}
		else {
			cout << "Audio file " <<it->first << " loaded." << endl;
		}
	}
	return true;
}