#pragma once
#include <MainSrc/Map/Map.h>
#include <unordered_map>
#include "Character.h"
#include "AnimationKey.h"

class Player : public Character
{
public:
	Player(std::shared_ptr<SpriteAnimationPlayer> sprite);
	~Player();

	void Init() override;
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
	void Update(float deltatime) override;

	void HandleInput(int keyMask) override;

private:
	static const std::unordered_map<AnimationKey, std::pair<int, int>> s_AnimationMap;
};