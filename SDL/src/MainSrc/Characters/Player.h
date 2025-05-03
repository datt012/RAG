#pragma once
#include "Character.h"

class Player : public Character
{
protected:
	void SetCurrentAction(Action action);

	void ApplyMovement(float deltaTime) override;
	void UpdateAnimation(float deltaTime) override;

public:
	Player(std::shared_ptr<SpriteAnimationPlayer> sprite);
	~Player();

	void Init() override;
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
	void Update(float deltatime) override;

	void HandleInput(int keyMask) override;


};