#pragma once
#include "SDL_mixer.h"
#include <string>
#include <map>
#include "Singleton.h"
class Sound : public CSingleton<Sound> {
public:
	Sound();
	~Sound();
	bool Init();
	void PlaySound(const std::string& name);
	void PauseSound();
	void ResumeSound();
	void StopSound();
	void LoadSound(const std::string& name);
	void LoadSfx(const std::string& name);
	void PlaySfx(const std::string& name, int repeat = 0);
	void CleanUp();
	void SetFlag(bool v) {
		m_flag = v;
	}
	void SetSFXFlag(bool v) {
		m_SFXFlag = v;
	}
	bool GetFlag() {
		return m_flag;
	}
	bool GetSFXFlag() {
		return m_SFXFlag;
	}
private:
	std::string m_PathMusic;
	std::map< std::string, Mix_Chunk*> m_Sfxs;
	std::map<std::string, Mix_Music*> m_Sounds;
	bool m_flag;
	bool m_SFXFlag;
};