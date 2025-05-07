#pragma once
#include "BaseObject2d.h"
#include "MainSrc/Characters/Player.h"
#include "SpriteAnimation.h"
#include "TextureManager.h"
#include "MainSrc/Entities/Laser.h"
class Enemy3 : public BaseObject2d {
public:
	void Init() override;
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
	void Init2(const std::shared_ptr<TextureManager> imgTexture, int health, float speed);
	void SetHealth(int health) {
		m_health = health;
	}
	void SetFposition(Vector2 vector) {
		m_Fposition = vector;
	}
	Vector2 GetFposition() {
		return m_Fposition;
	}
	void SetTargetPlayer(std::shared_ptr<Player> p) { m_targetPlayer = p; }


	/*void moveRight(float deltaTime);
	void moveLeft(float deltaTime);
	void moveUp(float deltaTime);
	void moveDown(float deltaTime);*/
private:;
	   int m_health;
	   float m_speed;
	   bool m_isLive;
	   Vector2 m_Fposition;
	   std::shared_ptr<Player> m_targetPlayer;


	   std::shared_ptr<SpriteAnimation> m_idleAnimation;
	   std::shared_ptr<SpriteAnimation> m_attackAnimation;
	   std::shared_ptr<SpriteAnimation> m_hurtAnimation;
	   std::shared_ptr<SpriteAnimation> m_dieAnimation;
	   std::shared_ptr<SpriteAnimation> m_runAnimation;
	   std::shared_ptr<SpriteAnimation> m_spellAnimation;

	   std::shared_ptr<SpriteAnimation> m_Animation;

	   
	   void UpdateAnimation();
	   /*void followPlayer(float deltaTime);
	   void Move(Vector2 pos, float deltaTime);
	   void Attack(float deltaTime);
	   void Handle(float deltaTime);*/

};