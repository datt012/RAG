#pragma once
#include <Singleton.h>
class Level : public CSingleton<Level> {
private:
	int level = 1;
	int maxLevel = 3;
	int minLevel = 1;
public:
	void SetLevel(int lv) {
		if (lv >= minLevel && lv <= maxLevel) {
			level = lv;
		}
	}
	int GetLevel() {
		return level;
	}
};