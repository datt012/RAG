#include "Enemy2.h"
#include "Define.h"
#include <iostream>
#include "GameManager/ResourceManagers.h"
using namespace std;
void Enemy2::Init() {

}
void Enemy2::Init2(const std::shared_ptr<TextureManager> imgTexture, int health, float speed) {
    m_health = health;
    m_speed = speed;
    m_isLive = true;


    if (imgTexture) {
        m_attackAnimation = std::make_shared<SpriteAnimation>(imgTexture, 1, 7, 4,  20.0f);
        m_attackAnimation->SetSize(72, 72);

        m_dieAnimation = std::make_shared<SpriteAnimation>(imgTexture, 2, 7, 4,  20.0f);
        m_dieAnimation->SetSize(72, 72);

        m_idleAnimation = std::make_shared<SpriteAnimation>(imgTexture, 3, 7, 4,  20.0f);
        m_idleAnimation->SetSize(72, 72);

        m_hitAnimation = std::make_shared<SpriteAnimation>(imgTexture, 4, 7, 4, 1.0f);
        m_hitAnimation->SetSize(72, 72);
        
        if (m_idleAnimation) m_idleAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_attackAnimation) m_attackAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_hitAnimation) m_hitAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_dieAnimation) m_dieAnimation->Set2DPosition(m_position.x, m_position.y);

        m_Animation = m_idleAnimation;
    }

}
void Enemy2::Update(float deltaTime) {
   /* Vector2 pos = m_targetPlayer->Get2DPosition();
    Move(pos, deltaTime);*/
    //moveUp(deltaTime);
    //moveDown(deltaTime);
    Handle(deltaTime);
    if (m_Animation) {
        m_Animation->Update(deltaTime);
    }
    for (auto& laser : m_listLasers) {
        laser->Update(deltaTime);
    }
}

void Enemy2::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    if (m_Animation) {
        m_Animation->Draw(renderer);
    }
    for (auto& laser : m_listLasers) {
        laser->Draw(renderer);
    }

}
void Enemy2::UpdateAnimation() {
    if (m_idleAnimation) m_idleAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_attackAnimation) m_attackAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_hitAnimation) m_hitAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_dieAnimation) m_dieAnimation->Set2DPosition(m_position.x, m_position.y);

    if (m_Animation) m_Animation->Set2DPosition(m_position.x, m_position.y);
}

void Enemy2::moveLeft(float deltaTime)
{
    m_flip = SDL_FLIP_NONE;


    m_position.x = m_position.x - m_speed * deltaTime / 3;


    if (m_Animation) {
        m_Animation->SetFlip(SDL_FLIP_NONE);
        UpdateAnimation();
    }



    if (m_Animation != m_idleAnimation)
    {
        m_Animation = m_idleAnimation;
    }
}

void Enemy2::moveRight(float deltaTime)
{
    m_flip = SDL_FLIP_HORIZONTAL;
    m_position.x = m_position.x + m_speed * deltaTime / 3;


    if (m_Animation) {
        m_Animation->SetFlip(SDL_FLIP_HORIZONTAL);
        UpdateAnimation();
    }



    if (m_Animation != m_idleAnimation)
    {
        m_Animation = m_idleAnimation;
    }
}
void Enemy2::moveUp(float deltaTime) {
    m_flip = SDL_FLIP_HORIZONTAL;
    m_position.y = m_position.y - m_speed * deltaTime / 3;


    if (m_Animation) {
        m_Animation->SetFlip(SDL_FLIP_HORIZONTAL);
        UpdateAnimation();
    }



    if (m_Animation != m_idleAnimation)
    {
        m_Animation = m_idleAnimation;
    }
}

void Enemy2::moveDown(float deltaTime) {
    m_flip = SDL_FLIP_HORIZONTAL;
    m_position.y = m_position.y + m_speed * deltaTime / 3;


    if (m_Animation) {
        m_Animation->SetFlip(SDL_FLIP_HORIZONTAL);
        UpdateAnimation();
    }



    if (m_Animation != m_idleAnimation)
    {
        m_Animation = m_idleAnimation;
    }
}

void Enemy2::followPlayer(float deltaTime) {
    
}
void Enemy2::Move(Vector2 pos, float deltaTime) {

   
    int disX = pos.x - m_position.x;
    int disY = pos.y - m_position.y;
    if (disX > 0) {
        moveRight(deltaTime);
    }
    else {
        moveLeft(deltaTime);
    }

    if (disY > 0) {
        moveDown(deltaTime);
    }
    else {
        moveUp(deltaTime);
    }
   
}

void Enemy2::Attack(float deltaTime) {
    static float timeCount = 0.0f;
    timeCount += deltaTime;
    if (timeCount < 1000.0f) return;
    else {
        UpdateAnimation();
        auto laserTexture = ResourceManagers::GetInstance()->GetTexture("a02.png");
        std::shared_ptr<Laser> laser = std::make_shared<Laser>();
        laser->Init2(laserTexture, LASER_SPEED/20);
        laser->SetSize(0.5f, 0.5f);
        if (m_position.x < m_targetPlayer->GetPosition().x) {

            Vector2 laserPos = m_position;
            laserPos.y += 20;
            laserPos.x += 1;
            laser->SetPosition(laserPos);
            laser->SetFlip(SDL_FLIP_VERTICAL);
            laser->SetAngle(90.0);
            laser->SetDirection(Vector2(0, +1));
            m_listLasers.push_back(laser);
        }
        else {

            Vector2 laserPos = m_position;
            laserPos.y += 20;
            laserPos.x += 1;
            laser->SetPosition(laserPos);
            laser->SetAngle(90.0);
            laser->SetDirection(Vector2(0, +1));
            m_listLasers.push_back(laser);
        }

        m_Animation = m_attackAnimation;
        UpdateAnimation();
        timeCount = 0.0f;
    }
}

void Enemy2::Handle(float deltaTime) {
    Vector2 playerPos = m_targetPlayer->Get2DPosition();

    float distanceFromOriginX = m_position.x - m_Fposition.x;

    
    if (abs(distanceFromOriginX) >= 300) {
        Move(m_Fposition, deltaTime);
        UpdateAnimation();
        cout << "1 " << endl;
        return;
    }

    float distanceToPlayerX = playerPos.x - m_position.x;
    float distanceToPlayerY = playerPos.y - m_position.y;

    // Nếu người chơi nằm trong khoảng X nhất định thì đuổi theo và tấn công
    if (abs(distanceToPlayerX) <= 150) {
        Vector2 targetPos = playerPos;
        targetPos.y -= 300; 
        Move(targetPos, deltaTime);
        Attack(deltaTime);
        UpdateAnimation();
        cout << "2 " << endl;
    }
    
}

