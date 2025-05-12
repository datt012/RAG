#pragma once
#include <unordered_map>
#include <algorithm>
#include "MainSrc/Characters/KeyDirection.h"
#include "MainSrc/Collision/Collision.h"
#include "MainSrc/Characters/Character.h"
#include "MainSrc/Characters/AnimationKey.h"
#include "ResourceManagers.h"

class Player : public Character
{
public:
	Player(std::shared_ptr<SpriteAnimationPlayer> sprite);
	~Player();

	void Init() override;
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
	void Update(float deltatime) override;

	void HandleInput(int keyMask) override;
	void Shoot() override;

	// Collider
	virtual SDL_Rect GetColliderRect() override;
	virtual SDL_FRect GetColliderFRect() override;

private:
	static const std::unordered_map<AnimationKey, std::pair<int, int>> s_AnimationMap;
};