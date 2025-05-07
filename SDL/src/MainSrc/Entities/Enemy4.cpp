#include "Enemy4.h"
#include "Define.h"
#include <iostream>
#include "GameManager/ResourceManagers.h"
using namespace std;
void Enemy4::Init() {

}
void Enemy4::Init2(const std::shared_ptr<TextureManager> imgTexture, int health, float speed) {
    m_health = health;
    m_speed = speed;
    m_isLive = true;


    if (imgTexture) {
        m_Animation = std::make_shared<SpriteAnimation>(imgTexture, 1, 5, 1, 100.0f);
        m_Animation->SetSize(48, 48);

        





        if (m_Animation) m_Animation->Set2DPosition(m_position.x, m_position.y);
        
    }

}
void Enemy4::Update(float deltaTime) {
    /* Vector2 pos = m_targetPlayer->Get2DPosition();
     Move(pos, deltaTime);*/
     //moveUp(deltaTime);
     //moveDown(deltaTime);

    if (m_Animation) {
        m_Animation->Update(deltaTime);
    }

}

void Enemy4::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    if (m_Animation) {
        m_Animation->Draw(renderer);
    }


}
void Enemy4::UpdateAnimation() {

    if (m_Animation) m_Animation->Set2DPosition(m_position.x, m_position.y);
}