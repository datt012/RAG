#pragma once
#include <SDL.h>
#include <memory>
#include "SpriteAnimation.h"
#include <SpriteAnimationPlayer.h>
#include <Timer.h>
#include <map>
#include <MainSrc/Map/Map.h>
#include "DirectionGun.h"

class Character : public BaseObject
{
protected:
    DirectionGun m_CurrentDirectionGun;

    // Main sprite (or animation)
    std::shared_ptr<SpriteAnimationPlayer> m_animationPlayer;

    Vector2     m_Velocity = { 0, 0 };
    Vector2     m_Displacement = { 0, 0 };

    float m_Speed = 0.17f;           // Running speed
    float m_JumpForce = 0.3f;      // Jumping force
    float m_JumpDuration = 0.4f;    // Jumping duration
    float m_Gravity = 0.0007f;       // Gravity

    bool m_IsOnGround = false;
    bool m_IsJumping = false;       // Flag for jumping
    bool m_IsShooting = false;      // Flag for shooting

public:
    Character(std::shared_ptr<SpriteAnimationPlayer> sprite) :
        m_animationPlayer(sprite) {
    }
    ~Character() {}

    virtual void	SetSize(int width, int height);
    virtual int     GetWidth();
    virtual int     GetHeight();

    virtual Vector3     GetPosition() override;
    virtual void        SetPosition(Vector3 position) override;
    virtual Vector2		Get2DPosition();
    virtual void		Set2DPosition(float x, float y);
    virtual void		SetRotation(double angle);
    virtual void		SetFlip(SDL_RendererFlip flip);

    virtual void        SetVelocity(Vector2 velocity);
    virtual void        SetVelocity(float vx, float vy);
    virtual Vector2     GetVelocity();

    // Handle input in bitmask format from external class (GSPlay)
    virtual void HandleInput(int keyMask) = 0;

    // Parameters that can be adjusted or overridden by subclasses
    virtual void SetSpeed(float speed);
    virtual void SetJumpForce(float force);
    virtual void SetJumpDuration(float duration);
    virtual void SetGravity(float gravity);

    // Collider
    virtual SDL_Rect GetColliderRect();
    virtual SDL_FRect GetColliderFRect();

    virtual void ClampPositionToMapBounds(std::shared_ptr<Map> map);
    virtual void CollisionWithMap(std::shared_ptr<Map> map);
    virtual void UpdateIsOnGround(std::shared_ptr<Map> map);
    virtual void SolveCollision(std::shared_ptr<Map> map);
};

