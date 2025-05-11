#pragma once
#include "SDL.h"
#include <vector>
#include "MainSrc/Entities/Enemy.h"
#include "MainSrc/Entities/Enemy2.h"
#include "MainSrc/Entities/Enemy3.h"
#include "MainSrc/Entities/Enemy4.h"
#include "MainSrc/Characters/Player.h"
#include "MainSrc/Map/Map.h"
class Level {
private:
	int m_level;//
	std::vector<std::shared_ptr <Enemy>> m_listEnemy1;
	std::vector<std::shared_ptr <Enemy2>> m_listEnemy2;
	std::vector<std::shared_ptr <Enemy3>> m_listEnemy3;
	std::vector<std::shared_ptr <Enemy4>> m_listEnemy4;

	std::shared_ptr<Map> m_map;
	std::shared_ptr<Player> m_player;

public:
	Level(int level );
	~Level();

	void Init(std::shared_ptr<Player> p);
	void Update(float deltaTime);
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	
	void isComplete();
	void SetPlayer(std::shared_ptr<Player> p) {
		m_player = p;
	}
	std::shared_ptr<Map> GetMap() const { return m_map; }
	
};