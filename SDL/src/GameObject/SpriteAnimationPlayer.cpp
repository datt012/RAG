#include "SpriteAnimationPlayer.h"
#include "TextureManager.h"
#include "Camera.h"

SpriteAnimationPlayer::SpriteAnimationPlayer(std::shared_ptr<TextureManager> texture, 
		int spriteSheetRows, int spriteSheetCols, int frameStartIndex, int frameEndIndex,
		float frameTime, bool loop) :
	BaseObject(texture),
	m_spriteSheetRows(spriteSheetRows),
	m_spriteSheetCols(spriteSheetCols),
	m_frameStartIndex(frameStartIndex),
	m_frameEndIndex(frameEndIndex),
	m_frameTime(frameTime),
	m_loop(loop)
{
	m_currentFrame = m_frameStartIndex;
	m_currentTicks = 0;
	m_lastUpdate = SDL_GetTicks();
	Init();
}
SpriteAnimationPlayer::~SpriteAnimationPlayer()
{

}

void SpriteAnimationPlayer::Init()
{

}

void SpriteAnimationPlayer::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{
	if (m_pTexture != nullptr)
	{
		float srcWidth = m_pTexture->GetOriginalWidth() / m_spriteSheetCols;
		float srcHeight = m_pTexture->GetOriginalHeight() / m_spriteSheetRows;

		int rowFrame = m_currentFrame / m_spriteSheetCols;
		int colFrame = m_currentFrame % m_spriteSheetCols;

		SDL_Rect srcRect = { srcWidth * colFrame, srcHeight * rowFrame, srcWidth, srcHeight }; //pointer to source rect(the area and position where you get the sprite on the texture).
		SDL_Rect dstRect = { m_position.x - Camera::GetInstance()->GetPosition().x, m_position.y - Camera::GetInstance()->GetPosition().y, m_iWidth , m_iHeight }; // pointer to dest rect(the area and position on the renderer you are going to draw).

		if (dstRect.x > SCREEN_WIDTH || dstRect.y > SCREEN_HEIDHT ||
			dstRect.x + dstRect.w < 0 || dstRect.y + dstRect.h < 0) return;
		
		SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), m_pTexture->GetTextureObj(), &srcRect, &dstRect, m_angle, nullptr, m_flip);
	}
}

void SpriteAnimationPlayer::Update(float deltatime)
{
	m_currentTicks += deltatime;
	if (m_currentTicks >= m_frameTime) {
		if (m_currentFrame <= m_frameEndIndex) {
			m_currentFrame++;
		}

		if (m_currentFrame > m_frameEndIndex) {
			m_currentFrame = m_loop ? m_frameStartIndex : m_frameEndIndex;
		}

		m_currentTicks -= m_frameTime;
	}
}

void SpriteAnimationPlayer::SetFrame(int startFrameIndex, int endFrameIndex)
{
	m_frameStartIndex = startFrameIndex;
	m_frameEndIndex = endFrameIndex;

	if (m_currentFrame < m_frameStartIndex || m_currentFrame > m_frameEndIndex)
	{
		m_currentFrame = m_frameStartIndex;
	}
}

void SpriteAnimationPlayer::Set2DPosition(float x, float y)
{
	m_position = Vector3((float)x, (float)y, 0.0f);
}

void SpriteAnimationPlayer::SetRotation(double angle)
{
	m_angle = angle;
}

void SpriteAnimationPlayer::SetFlip(SDL_RendererFlip flip)
{
	m_flip = flip;
}

Vector2 SpriteAnimationPlayer::Get2DPosition()
{
	return Vector2(m_position.x, m_position.y);
}

void SpriteAnimationPlayer::SetSize(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_scale = Vector3((float)m_iWidth, (float)m_iHeight, 0.0f);
}

int SpriteAnimationPlayer::GetWidth()
{
	return m_iWidth;
}

int SpriteAnimationPlayer::GetHeight()
{
	return m_iHeight;
}

//int SpriteAnimationPlayer::GetCurrentAction()
//{
//	return m_currentAction;
//}
//
//void SpriteAnimationPlayer::SetCurrentAction(int action)
//{
//	m_currentAction = action;
//}
//
//int SpriteAnimationPlayer::GetFrameCount()
//{
//	return m_frameCount;
//}
//
//void SpriteAnimationPlayer::SetFrameCount(int frameCount)
//{
//	m_frameCount = frameCount;
//}
