#include "Sound.h"
#include <SDL_ttf.h>
Sound::Sound() {
    m_PathMusic = "Data/Sounds/";
    Init();
}
Sound::~Sound() {
    CleanUp();
}
bool Sound::Init() {   
    m_flag = true;
    m_SFXFlag = true;
    bool ret = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        ret = false; 
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        ret = false;
    }
    return ret;
}
void Sound::PlaySound(const std::string& name) {
    if (m_flag == false) return;
    Mix_Music* music = NULL;
    auto it = m_Sounds.find(name);
    if (it != m_Sounds.end()) {
        music = it->second;
    }
    if (Mix_PlayingMusic() == 0) {
        if (music != nullptr) {
            Mix_PlayMusic(music, -1); 
        }
        else {
            printf("No sound available can play");
        }
    }
    else {
        if (Mix_PausedMusic() == 1) {
            Mix_ResumeMusic();
        }
        else {
            Mix_PauseMusic();
        }
    }
}
void Sound::PauseSound() {
    Mix_PauseMusic();
}
void Sound::ResumeSound() {
    Mix_ResumeMusic();
}
void Sound::StopSound() {
    Mix_HaltMusic();
}
void Sound::LoadSound(const std::string& name) {
    std::string pathFile = m_PathMusic + name;
    auto it = m_Sounds.find(name);
    if (it != m_Sounds.end()) return;
    Mix_Music* m_Music = Mix_LoadMUS(pathFile.c_str());
    if (m_Music != nullptr) {
        m_Sounds.insert(std::pair<const std::string&, Mix_Music*>(name, (m_Music)));
    }
    else {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
}
void Sound::LoadSfx(const std::string& name) {
    auto it = m_Sfxs.find(name);
    if (it != m_Sfxs.end()) return;
	unsigned int ret = 0;
    std::string path = m_PathMusic + name;
	Mix_Chunk * chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr) {
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else {
		m_Sfxs.insert(std::pair<const std::string&, Mix_Chunk*>(name,(chunk)));	
	}
}
void Sound::PlaySfx(const std::string& name, int repeat) {   
    if (m_SFXFlag == false) return;
    Mix_Chunk* chunk = NULL;
    auto it = m_Sfxs.find(name);
    if (it != m_Sfxs.end()) {
        chunk = it->second;
    }
	if (chunk!= nullptr) {
		Mix_PlayChannel(-1, chunk, repeat);
	}
    else {
        printf("Cannot play sfx");
    }
}
void Sound::CleanUp() {
	for (auto it = m_Sfxs.begin(); it != m_Sfxs.end(); it++) {
		Mix_FreeChunk(it->second);
	}
	m_Sfxs.clear();
    for (auto it = m_Sounds.begin(); it != m_Sounds.end(); it++) {
        Mix_FreeMusic(it->second);
    }
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
