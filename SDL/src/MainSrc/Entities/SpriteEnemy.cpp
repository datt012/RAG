#include "SpriteEnemy.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Renderer.h"

SpriteEnemy::SpriteEnemy(std::shared_ptr<TextureManager> texture, int frameCount, float frameTime)
    : BaseObject(texture),
    m_frameCount(frameCount),
    m_frameTime(frameTime),
    m_currentFrame(0),
    m_startFrame(0),
    m_endFrame(frameCount - 1),
    m_currentTicks(0.0f)
{
    Init();
}

SpriteEnemy::~SpriteEnemy()
{
}

void SpriteEnemy::Init()
{
    m_currentFrame = m_startFrame;
    m_currentTicks = 0.0f;
}

void SpriteEnemy::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{
    if (m_pTexture)
    {
        float srcWidth = (float)m_pTexture->GetOriginalWidth() / m_frameCount;
        float srcHeight = (float)m_pTexture->GetOriginalHeight(); // Vì các frame nằm trên 1 hàng
        SDL_Rect srcRect = { static_cast<int>(srcWidth * m_currentFrame), 0, static_cast<int>(srcWidth), static_cast<int>(srcHeight) };
        SDL_Rect dstRect = { static_cast<int>(m_position.x - Camera::GetInstance()->GetPosition().x),
                             static_cast<int>(m_position.y - Camera::GetInstance()->GetPosition().y),
                             m_iWidth, m_iHeight };

        SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), m_pTexture->GetTextureObj(),
            &srcRect, &dstRect, m_angle, nullptr, m_flip);
    }
}

void SpriteEnemy::Update(float deltaTime)
{
    m_currentTicks += deltaTime;

    if (m_currentTicks >= m_frameTime)
    {
        m_currentFrame++;
        if (m_currentFrame > m_endFrame)
        {
            m_currentFrame = m_startFrame;
        }
        m_currentTicks -= m_frameTime;
    }
}

void SpriteEnemy::Set2DPosition(float x, float y)
{
    m_position = Vector3(x, y, 0.0f);
}

void SpriteEnemy::SetRotation(double angle)
{
    m_angle = angle;
}

void SpriteEnemy::SetFlip(SDL_RendererFlip flip)
{
    m_flip = flip;
}

Vector2 SpriteEnemy::Get2DPosition() const
{
    return Vector2(m_position.x, m_position.y);
}

void SpriteEnemy::SetSize(int width, int height)
{
    m_iWidth = width;
    m_iHeight = height;
    m_scale = Vector3((float)m_iWidth, (float)m_iHeight, 0.0f);
}

int SpriteEnemy::GetWidth() const
{
    return m_iWidth;
}

int SpriteEnemy::GetHeight() const
{
    return m_iHeight;
}

void SpriteEnemy::SetFrameRange(int startFrame, int endFrame)
{
    m_startFrame = startFrame;
    m_endFrame = endFrame;
    m_currentFrame = m_startFrame;
}

bool SpriteEnemy::IsAnimationFinished() const
{
    // Animation finish khi frame cuối và đủ thời gian frame
    return (m_currentFrame == m_endFrame) && (m_currentTicks >= m_frameTime);
}
