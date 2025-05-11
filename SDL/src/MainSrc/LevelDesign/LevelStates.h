#pragma once
#include <vector>
#include <Singleton.h>

class LevelStates : public CSingleton <LevelStates> {
private:
	int m_levelState = 1;
	int maxLevel = 3;
	int minLevel = 1;
public:
	void SetLevelState(int level) {
		if (level >= minLevel && level <= maxLevel) {
			m_levelState = level;
		}
	}

	int GetLevelState() {
		return m_levelState;
	}
	void NextLevelState() {
		if (m_levelState < maxLevel) {
			++m_levelState;
		}
	}

	void PreLevelState() {
		if (m_levelState > minLevel) {
			--m_levelState;
		}
	}
};
