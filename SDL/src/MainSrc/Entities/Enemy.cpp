#include "Enemy.h"
#include "TextureManager.h"
#include "GameManager/ResourceManagers.h"
#include "Define.h"
Enemy::Enemy()
    : m_health(100), m_speed(0.1f), m_isAlive(true)
{
}

Enemy::~Enemy()
{
}
void Enemy::Init() {

}
void Enemy::Init2(std::shared_ptr<TextureManager> texture, int health, float speed)
{
    m_health = health;
    m_speed = speed;
    m_isAlive = true;

    if (texture)
    {
        // Idle Animation
        m_idleAnimation = std::make_shared<SpriteEnemy>(texture, 24, 50.0f);
        m_idleAnimation->SetFrameRange(0, 3); // Frame 0 -> 3
        m_idleAnimation->SetSize(48, 48);

        // Run Animation
        m_runAnimation = std::make_shared<SpriteEnemy>(texture, 24, 100.0f);
        m_runAnimation->SetFrameRange(4, 9); // Frame 4 -> 9
        m_runAnimation->SetSize(48, 48);

        // Attack Animation
        m_attackAnimation = std::make_shared<SpriteEnemy>(texture, 24, 30.0f);
        m_attackAnimation->SetFrameRange(10, 15); // Frame 10 -> 15
        m_attackAnimation->SetSize(48, 48);

        // Hit Animation
        m_hitAnimation = std::make_shared<SpriteEnemy>(texture, 24, 30.0f);
        m_hitAnimation->SetFrameRange(16, 18); // Frame 16 -> 18
        m_hitAnimation->SetSize(48, 48);

        // Die Animation
        m_dieAnimation = std::make_shared<SpriteEnemy>(texture, 24, 30.0f);
        m_dieAnimation->SetFrameRange(19, 23); // Frame 19 -> 23
        m_dieAnimation->SetSize(48, 48);

       

        // Đặt animation hiện tại là Idle
        m_currentAnimation = m_idleAnimation;

        if (m_idleAnimation) m_idleAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_runAnimation) m_runAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_attackAnimation) m_attackAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_hitAnimation) m_hitAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_dieAnimation) m_dieAnimation->Set2DPosition(m_position.x, m_position.y);

    }
}
void Enemy::dongBoViTri() {
    if (m_idleAnimation) m_idleAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_runAnimation) m_runAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_attackAnimation) m_attackAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_hitAnimation) m_hitAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_dieAnimation) m_dieAnimation->Set2DPosition(m_position.x, m_position.y);
}
void Enemy::Update(float deltaTime)
{   
    followPlayer(deltaTime);
    if (m_currentAnimation)
    {
        m_currentAnimation->Update(deltaTime);
    }
    for (auto& laser : m_listLaser) {
        laser->Update(deltaTime);
    }
}

void Enemy::Draw(SDL_Renderer* renderer, SDL_Rect* clip )
{
    if (m_currentAnimation)
    {
        m_currentAnimation->Draw(renderer);
    }
    for (auto& laser : m_listLaser) {
        laser->Draw(renderer);
    }
}



void Enemy::PlayIdle()
{
    m_currentAnimation = m_idleAnimation;
}

void Enemy::PlayRun()
{
    m_currentAnimation = m_runAnimation;
}

void Enemy::PlayAttack()
{
    m_currentAnimation = m_attackAnimation;
}

void Enemy::PlayHit()
{
    m_currentAnimation = m_hitAnimation;
}

void Enemy::PlayDie()
{
    m_currentAnimation = m_dieAnimation;
    m_isAlive = false;
}

bool Enemy::IsAlive() const
{
    return m_isAlive;
}



void Enemy::handleAll(float deltaTime) {

}
void Enemy::moveLeft(float deltaTime)
{   
    m_flip = SDL_FLIP_HORIZONTAL;
   
    
    m_position.x = m_position.x -  m_speed * deltaTime/5;

    
    if (m_currentAnimation) {
        m_currentAnimation->SetFlip(SDL_FLIP_HORIZONTAL);
        dongBoViTri();
    }
       

   
    if (m_currentAnimation != m_runAnimation)
    {
        PlayAttack();
    }
}

void Enemy::moveRight(float deltaTime)
{   
    m_flip = SDL_FLIP_NONE;
    m_position.x = m_position.x + m_speed * deltaTime / 5;

   
    if (m_currentAnimation) {
        m_currentAnimation->SetFlip(SDL_FLIP_NONE);
        dongBoViTri();
    }
        

    
    if (m_currentAnimation != m_runAnimation)
    {
        PlayAttack();
    }
}
float Enemy::dsPlayer()
{
    if (m_targetPlayer)
    {
        Vector2 playerPos = { m_targetPlayer->GetPosition().x, m_targetPlayer->GetPosition().y };
        Vector2 enemyPos = m_position;

        float dx = playerPos.x - enemyPos.x;
        float dy = playerPos.y - enemyPos.y;
        float ds = sqrt(dx * dx + dy * dy);
        printf("%f\n", ds);
        return sqrt(dx * dx + dy * dy);
    }
    return -1.0f; 
}
void Enemy::followPlayer(float deltaTime)
{
    if (m_targetPlayer && m_isAlive)
    {
        Vector2 playerPos = { m_targetPlayer->GetPosition().x, m_targetPlayer->GetPosition().y };
        Vector2 enemyPos = m_position;
        if (abs(playerPos.x - enemyPos.x) <= 60.0f) {
            if (playerPos.x > enemyPos.x) {
                m_currentAnimation->SetFlip(SDL_FLIP_NONE);
            }
            else {
                m_currentAnimation->SetFlip(SDL_FLIP_HORIZONTAL);
            }
            Attack();
            dongBoViTri();
        }
        else {
            if (playerPos.x <= enemyPos.x)
            {

                moveLeft(deltaTime);
            }
            else if (playerPos.x > enemyPos.x)
            {

                moveRight(deltaTime);
            }
        }
        
    }
}
void Enemy::Attack()
{
    dongBoViTri();
    auto laserTexture = ResourceManagers::GetInstance()->GetTexture("a13.png");
    std::shared_ptr<Laser> laser = std::make_shared<Laser>();
    laser->Init2(laserTexture, LASER_SPEED);
    Vector2 laserPos = m_position;
    laserPos.y += 20;
    laser->SetPosition(laserPos);
    laser->SetSize(0.1f, 0.1f);
    if (m_position.x < m_targetPlayer->GetPosition().x) {
        
        
        laser->SetFlip(SDL_FLIP_NONE);
        laser->SetDirection(Vector2(1, 0));
        m_listLaser.push_back(laser);
    }
    else {
        laser->SetFlip(SDL_FLIP_HORIZONTAL);
        laser->SetDirection(Vector2(-1, 0));
        m_listLaser.push_back(laser);
    }

    m_currentAnimation = m_attackAnimation;
    dongBoViTri();
}