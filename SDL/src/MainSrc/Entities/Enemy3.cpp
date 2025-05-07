#include "Enemy3.h"
#include "Define.h"
#include <iostream>
#include "GameManager/ResourceManagers.h"
using namespace std;
void Enemy3::Init() {

}
void Enemy3::Init2(const std::shared_ptr<TextureManager> imgTexture, int health, float speed) {
    m_health = health;
    m_speed = speed;
    m_isLive = true;


    if (imgTexture) {
        m_attackAnimation = std::make_shared<SpriteAnimation>(imgTexture, 1, 8, 6, 0.15f);
        m_attackAnimation->SetSize(100, 100);

        m_dieAnimation = std::make_shared<SpriteAnimation>(imgTexture, 2, 8, 6, 2.0f);
        m_dieAnimation->SetSize(100, 100);



        m_idleAnimation = std::make_shared<SpriteAnimation>(imgTexture, 3, 8, 6, 0.5f);
        m_idleAnimation->SetSize(100, 100);


        m_spellAnimation = std::make_shared<SpriteAnimation>(imgTexture, 4, 8, 6, 10.0f);
        m_spellAnimation->SetSize(100, 100);

        m_runAnimation = std::make_shared<SpriteAnimation>(imgTexture, 5, 8, 6, 0.1f);
        m_runAnimation->SetSize(100, 100);

       
        m_hurtAnimation = std::make_shared<SpriteAnimation>(imgTexture, 6, 8, 6, 0.1f);
        m_hurtAnimation->SetSize(100, 100);
        

        if (m_idleAnimation) m_idleAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_attackAnimation) m_attackAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_hurtAnimation) m_hurtAnimation->Set2DPosition(m_position.x, m_position.y);
        if (m_dieAnimation) m_dieAnimation->Set2DPosition(m_position.x, m_position.y);

        m_Animation = m_attackAnimation;
    }

}
void Enemy3::Update(float deltaTime) {
    /* Vector2 pos = m_targetPlayer->Get2DPosition();
     Move(pos, deltaTime);*/
     //moveUp(deltaTime);
     //moveDown(deltaTime);
   
    if (m_Animation) {
        m_Animation->Update(deltaTime);
    }
    
}

void Enemy3::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    if (m_Animation) {
        m_Animation->Draw(renderer);
    }
    

}
void Enemy3::UpdateAnimation() {
    if (m_idleAnimation) m_idleAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_attackAnimation) m_attackAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_hurtAnimation) m_hurtAnimation->Set2DPosition(m_position.x, m_position.y);
    if (m_dieAnimation) m_dieAnimation->Set2DPosition(m_position.x, m_position.y);

    if (m_Animation) m_Animation->Set2DPosition(m_position.x, m_position.y);
}