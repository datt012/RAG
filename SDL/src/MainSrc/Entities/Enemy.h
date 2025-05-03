#pragma once
#include <memory>
#include "SpriteEnemy.h"
#include "GameObject/CMath.h"
#include "MainSrc/Characters/Player.h"
#include "GameObject/BaseObject2d.h"
#include "MainSrc/Entities/Laser.h"
#include <vector>

class TextureManager;

class Enemy : public BaseObject2d{
private:
    int m_health;
    float m_speed;
    bool m_isAlive;

    std::shared_ptr<SpriteEnemy> m_idleAnimation;
    std::shared_ptr<SpriteEnemy> m_runAnimation;
    std::shared_ptr<SpriteEnemy> m_attackAnimation;
    std::shared_ptr<SpriteEnemy> m_hitAnimation;
    std::shared_ptr<SpriteEnemy> m_dieAnimation;

    std::shared_ptr<SpriteEnemy> m_currentAnimation;
    
    std::shared_ptr<Player> m_targetPlayer;
    
    std::vector<std::shared_ptr<Laser>> m_listLaser;
    float attackCountDown = 0.0f;

    float dsPlayer();
    Vector2 m_Fpositon;
public:
    Enemy();
    ~Enemy();

    void Init2(std::shared_ptr<TextureManager> texture, int health, float speed);
    void Init() override;
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
    

    void PlayIdle();
    void PlayRun();
    void PlayAttack();
    void PlayHit();
    void PlayDie();

    bool IsAlive() const;

    void SetTargetPlayer(std::shared_ptr<Player> player) {
        m_targetPlayer = player;
    }
    void SetFPositon(Vector2 post) {
        m_Fpositon = post;
    }
    Vector2 GetFpositon() {
        return m_Fpositon;
    }


    void handleAll(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void followPlayer(float deltaTime);
    void dongBoViTri();
    void Attack();

    
};
