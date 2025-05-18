#pragma once
#include <SDL.h>
#include <memory>
#include <map>
#include "SpriteAnimation.h"
#include "SpriteAnimationPlayer.h"
#include "Timer.h"
#include "MainSrc/Map/Map.h"
#include "DirectionGun.h"
#include "MainSrc/Projectiles/BulletPool.h"
class Character : public BaseObject {
protected:
    int m_HP;
    int m_MAX_HP;
    int m_ShootCooldown = 0;
    int m_ShootCooldownTime;
    std::shared_ptr<BulletPool> m_BulletPool;
    DirectionGun m_CurrentDirectionGun;
    std::shared_ptr<SpriteAnimationPlayer> m_animationPlayer;
    Vector2 m_Velocity = { 0, 0 };
    Vector2 m_Displacement = { 0, 0 };
    float m_Speed = 0;        
    float m_JumpForce = 0;     
    float m_Gravity = 0;   
    bool m_IsOnGround = false;
    bool m_IsJumping = false;     
    bool m_IsShooting = false;   
	void DrawCollider(SDL_Renderer* renderer, SDL_Color lineColor = { 255, 0, 0, 255 });
    void DrawHPBar(SDL_Renderer* renderer, const Vector2& position, int currentHP, int maxHP,
        int barWidth, int barHeight, 
        SDL_Color bgColor = { 100, 100, 100, 255 }, 
        SDL_Color hpColor = { 255, 0, 0, 255 },
        bool isWorldPosition = true);
public:
    Character(std::shared_ptr<SpriteAnimationPlayer> sprite) :
        m_animationPlayer(sprite) {
    }
    ~Character() {}
    virtual void SetSize(int width, int height);
    virtual int GetWidth();
    virtual int GetHeight();
    virtual Vector3 GetPosition() override;
    virtual void SetPosition(Vector3 position) override;
    virtual Vector2	Get2DPosition();
    virtual void Set2DPosition(float x, float y);
    virtual void SetRotation(double angle);
    virtual void SetFlip(SDL_RendererFlip flip);
    virtual void SetVelocity(Vector2 velocity);
    virtual void SetVelocity(float vx, float vy);
    virtual Vector2 GetVelocity();
    virtual void HandleInput(int keyMask) = 0;
    virtual void Shoot() = 0;
    virtual void SetSpeed(float speed);
    virtual void SetJumpForce(float force);
    virtual void SetJumpDuration(float duration);
    virtual void SetGravity(float gravity);
    virtual SDL_Rect GetColliderRect();
    virtual SDL_FRect GetColliderFRect();
    virtual void ClampPositionToMapBounds(std::shared_ptr<Map> map);
    virtual bool CollisionWithMap(std::shared_ptr<Map> map);
    virtual void UpdateIsOnGround(std::shared_ptr<Map> map);
    virtual void SolveCollision(std::shared_ptr<Map> map);
    int GetHP() const;
	bool IsAlive() const;
    int GetMAXHP() const;
    int GetShootCooldown() const;
    int GetShootCooldownTime() const;
    std::shared_ptr<BulletPool> GetBulletPool() const;
    bool IsOnGround() const;
    bool IsJumping() const;
    bool IsShooting() const;
    void SetHP(int hp);
    void TakeDamage(int damage);
    void SetShootCooldown(int time);
    void SetOnGround(bool isOnGround);
    void SetJumping(bool isJumping);
    void SetShooting(bool isShooting);
};

