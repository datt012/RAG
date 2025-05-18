#pragma once
#include <memory>
#include "GameObject/Sprite2D.h"
#include "Camera.h"
class Bullet : public Sprite2D {
protected:
	int m_damage = 0;
	bool m_IsActive = false;
	Vector2 m_Velocity = { 0, 0 };
public:
	Bullet() : Sprite2D() {}
	Bullet(std::shared_ptr<TextureManager> texture, SDL_RendererFlip flip) : Sprite2D(texture, flip) {}
	~Bullet() {}
	void Init() override;
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
	void Update(float deltatime) override;
	bool IsActive() const;
	void Deactivate();
	virtual SDL_Rect getColliderRect();
	virtual SDL_FRect getColliderFRect();
	int GetDamage() const;
	Vector2 GetVelocity() const;
	void SetDamage(int damage);
	void SetVelocity(Vector2 velocity);
	void SetVelocity(float vx, float vy);
};