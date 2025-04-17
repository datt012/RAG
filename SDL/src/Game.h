#pragma once
#include "Timer.h"
#include "GameManager/Singleton.h"
class Game :public CSingleton<Game> {
public:
	bool Init();
	void Update(float deltaTime);
	void Render();
	void Run();
	Game();
	~Game();
private:

};
