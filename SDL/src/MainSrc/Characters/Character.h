#pragma once
#include <SDL.h>
#include <memory>
#include "SpriteAnimation.h"
#include <SpriteAnimationPlayer.h>
#include <Timer.h>
#include <map>

class Character : public BaseObject
{
protected:
    // Facing direction
    enum Direction {
        LEFT,
        RIGHT
    } m_Facing = RIGHT;

    enum Action {
        IDLE,
        RUN,
        CROUCH,
        FALL
    } m_CurrentAction = IDLE;

    enum DirectionGun {
        DIR_HORIZONTAL,
        DIR_UP,
        DIR_DOWN,
        DIR_DIAGONAL_UP,
        DIR_DIAGONAL_DOWN
    } m_CurrentDirectionGun = DIR_HORIZONTAL;

    // Main sprite (or animation)
    std::shared_ptr<SpriteAnimationPlayer> m_animationPlayer;

    float       m_VelocityX = 0.0f;       // Velocity along the X-axis
    float       m_VelocityY = 0.0f;       // Velocity along the Y-axis

    float m_Speed = 0.3f;           // Running speed
    float m_JumpForce = 0.3f;      // Jumping force
    float m_JumpDuration = 0.4f;    // Jumping duration
    float m_Gravity = 0.0007f;       // Gravity

    bool m_IsJumping = false;       // Flag for jumping
    bool m_IsShooting = false;      // Flag for shooting

    // Private functions to handle movement/state logic
    virtual void ApplyMovement(float deltaTime) = 0;
    virtual void UpdateAnimation(float deltaTime) = 0;

public:
    Character(std::shared_ptr<SpriteAnimationPlayer> sprite) :
        m_animationPlayer(sprite) {
    }
    ~Character() {}

    virtual void	SetSize(int width, int height) { m_animationPlayer->SetSize(width, height); }
    virtual int     GetWidth() { return m_animationPlayer->GetWidth(); }
    virtual int     GetHeight() { return m_animationPlayer->GetHeight(); }

    virtual Vector3     GetPosition() override { return m_animationPlayer->GetPosition(); }
    virtual void        SetPosition(Vector3 position) override { m_animationPlayer->SetPosition(position); }
    virtual Vector2		Get2DPosition() { return m_animationPlayer->Get2DPosition(); }
    virtual void		Set2DPosition(float x, float y) { this->SetPosition(Vector3((float)x, (float)y, 0.0f)); }
    virtual void		SetRotation(double angle) { m_animationPlayer->SetRotation(angle); }
    virtual void		SetFlip(SDL_RendererFlip flip) { m_animationPlayer->SetFlip(flip); }

    virtual void SetVelocity(float vx, float vy) {
        m_VelocityX = vx;
        m_VelocityY = vy;
    }
    virtual void    SetVelocityX(float vx) { m_VelocityX = vx; }
    virtual void    SetVelocityY(float vy) { m_VelocityY = vy; }
    virtual int     GetVelocityX() { return m_VelocityX; }
    virtual int     GetVelocityY() { return m_VelocityY; }

    // Handle input in bitmask format from external class (GSPlay)
    virtual void HandleInput(int keyMask) = 0;

    // Parameters that can be adjusted or overridden by subclasses
    virtual void SetSpeed(float speed) { m_Speed = speed; }
    virtual void SetJumpForce(float force) { m_JumpForce = force; }
    virtual void SetJumpDuration(float duration) { m_JumpDuration = duration; }
    virtual void SetGravity(float gravity) { m_Gravity = gravity; }
};

