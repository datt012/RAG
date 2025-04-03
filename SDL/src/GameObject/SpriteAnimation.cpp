#include "SpriteAnimation.h"
#include "TextureManager.h"
#include "Camera.h"

SpriteAnimation::SpriteAnimation(std::shared_ptr<TextureManager> texture, int currentAction, int frameCount, int numAction, float frameTime) : BaseObject(texture)
{
	m_currentAction = currentAction;
	m_frameCount = frameCount;
	m_numAction = numAction;
	m_frameTime = frameTime;
	m_currentFrame = 0;
	m_currentTicks = 0;
	m_lastUpdate = SDL_GetTicks();
	Init();
}
SpriteAnimation::~SpriteAnimation()
{

}

void SpriteAnimation::Init()
{

}

void SpriteAnimation::Draw(SDL_Renderer* renderer, SDL_Rect * clip)
{
	if (m_pTexture != nullptr)
	{
		float srcWidth =  m_pTexture->GetOriginalWidth() / m_frameCount;
		float srcHeight = m_pTexture->GetOriginalHeight() / m_numAction;
		SDL_Rect srcRect = { srcWidth * m_currentFrame, srcHeight * (m_currentAction - 1), srcWidth, srcHeight }; //pointer to source rect(the area and position where you get the sprite on the texture).
		SDL_Rect dstRect = { m_position.x - Camera::GetInstance()->GetPosition().x, m_position.y - Camera::GetInstance()->GetPosition().y, m_iWidth , m_iHeight }; // pointer to dest rect(the area and position on the renderer you are going to draw).
		SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), m_pTexture->GetTextureObj(), &srcRect, &dstRect, m_angle, nullptr, m_flip);
		
	}
}

void SpriteAnimation::Update(float deltatime)
{
	m_currentTicks += deltatime;
		if(m_currentTicks  >= m_frameTime) {
		m_currentFrame++;
		if (m_currentFrame >= m_frameCount) {
			m_currentFrame = 0;
		}
		m_currentTicks -= m_frameTime;
	}
}

void SpriteAnimation::Set2DPosition(float x, float y)
{
	m_position = Vector3((float)x, (float)y, 0.0f);
}

void SpriteAnimation::SetRotation(double angle)
{
	m_angle = angle;
}

void SpriteAnimation::SetFlip(SDL_RendererFlip flip)
{ 
	m_flip = flip;
}

Vector2 SpriteAnimation::Get2DPosition()
{
	return Vector2(m_position.x, m_position.y);
}

void SpriteAnimation::SetSize(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_scale = Vector3((float)m_iWidth, (float)m_iHeight, 0.0f);
}

int SpriteAnimation::GetWidth()
{
	return m_iWidth;
}

int SpriteAnimation::GetHeight()
{
	return m_iHeight;
}

int SpriteAnimation::GetCurrentAction()
{
	return m_currentAction;
}

void SpriteAnimation::SetCurrentAction(int action)
{
	m_currentAction = action;
}

