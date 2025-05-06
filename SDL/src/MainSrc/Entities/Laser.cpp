#include "Laser.h"
#include "iostream"
#include "Camera.h"
Laser::Laser()
    : m_speed(0.0f), m_filePath("")
{
}

Laser::~Laser()
{
    
}

void Laser::Init()
{
   
}

void Laser::Init2(std::shared_ptr<TextureManager> bulletTexture, float speed)
{
    m_pTexture = bulletTexture;
    m_speed = speed;
}

void Laser::Update(float deltaTime)
{
   
    //m_position.x += m_speed * deltaTime;
    m_position.x = m_position.x + m_speed * deltaTime * m_direction.x;
    m_position.y = m_position.y + m_speed * deltaTime * m_direction.y;
}

void Laser::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{
    if (m_pTexture)
    {
        SDL_Texture* texture = m_pTexture->GetTextureObj();
        if (texture)
        {
            SDL_Rect dstRect;
            dstRect.x = static_cast<int>(m_position.x);
            dstRect.y = static_cast<int>(m_position.y);
            
            int w, h;
            SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
            dstRect.w = static_cast<int>(w * m_scale.x);
            dstRect.h = static_cast<int>(h * m_scale.y);

            SDL_Rect cam1 = Camera::GetInstance()->GetViewBox();
            dstRect.x -= cam1.x;
            dstRect.y -= cam1.y;
            SDL_RenderCopyEx(
                renderer,
                texture,
                clip,
                &dstRect,
                m_angle,
                nullptr,
                m_flip
            );
        }
    }
}

void Laser::SetSize(float x, float y)
{
    m_scale.x = x;
    m_scale.y = y;
}

