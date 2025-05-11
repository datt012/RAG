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

        m_hurtAnimation = std::make_shared<SpriteAnimation>(imgTexture, 4, 7, 4, 1.0f);
        m_hurtAnimation->SetSize(72, 72);
        
        if (m_idleAnimation) m_idleAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_attackAnimation) m_attackAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_hurtAnimation) m_hurtAnimation->Set2DPosition(m_position.x, m_position.y);
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
    if (m_hurtAnimation) m_hurtAnimation->Set2DPosition(m_position.x, m_position.y);
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
        auto laserTexture = ResourceManagers::GetInstance()->GetTexture("a.png");
        std::shared_ptr<Laser> laser = std::make_shared<Laser>();
        laser->Init2(laserTexture, LASER_SPEED/20);
        laser->SetSize(0.3f, 0.3f);
        if (m_position.x < m_targetPlayer->GetPosition().x) {

            Vector2 laserPos = m_position;
            laserPos.y += 20;
            laserPos.x += 25;
            laser->SetPosition(laserPos);
            laser->SetFlip(SDL_FLIP_VERTICAL);
            laser->SetAngle(90.0);
            laser->SetDirection(Vector2(0, +1));
            m_listLasers.push_back(laser);
        }
        else {

            Vector2 laserPos = m_position;
            laserPos.y += 20;
            laserPos.x += 25;
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
void Enemy2::Attack2(float deltaTime) {
    static float timeCount = 0.0f;
    timeCount += deltaTime;

   
    if (timeCount < 1000.0f) return;

    for (int i = 0; i <= 2; i++) {
        UpdateAnimation();

        auto laserTexture = ResourceManagers::GetInstance()->GetTexture("a.png");
        std::shared_ptr<Laser> laser = std::make_shared<Laser>();
        laser->Init2(laserTexture, LASER_SPEED / 20);
        laser->SetSize(0.3f, 0.3f);

        Vector2 laserPos = m_position;
        laserPos.y += 20;
        laserPos.x += 25;
        laser->SetPosition(laserPos);

        if (m_position.x < m_targetPlayer->GetPosition().x) {
            laser->SetFlip(SDL_FLIP_VERTICAL);
        }

        m_listLasers.push_back(laser);
    }

    
    if (m_listLasers.size() >= 3) {
        m_listLasers[m_listLasers.size() - 3]->SetDirection(Vector2(0, +1));    
        m_listLasers[m_listLasers.size() - 2]->SetDirection(Vector2(0.5, +1));    
        m_listLasers[m_listLasers.size() - 1]->SetDirection(Vector2(-0.5, +1));
    }

    m_Animation = m_attackAnimation;
    UpdateAnimation();
    timeCount = 0.0f;
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

        int lvState = LevelStates::GetInstance()->GetLevelState();
        if (lvState == 1) {
            Attack(deltaTime);
        }
        else if (lvState == 2) {
            Attack2(deltaTime);
        }
        else {
            // at3
            Attack2(deltaTime);
        }

        
        UpdateAnimation();
        cout << "2 " << endl;
    }
    
}

