#pragma once
#include "BaseObject.h"
#include <SDL_render.h>
class SpriteAnimationPlayer : public BaseObject {
protected:
	int	m_iHeight;
	int	m_iWidth;
	int	m_spriteSheetCols;
	int m_spriteSheetRows;
	int	m_frameStartIndex;
	int m_frameEndIndex;
	bool m_loop;
	int	m_currentFrame;
	float m_frameTime;
	float m_currentTime;
	float m_currentTicks;
	Uint32 m_lastUpdate;
	int	m_animSpeed;
public:
	SpriteAnimationPlayer(std::shared_ptr<TextureManager> texture, int spriteSheetRows, int spriteSheetCols, int frameStartIndex, int frameEndIndex, float frameTime, bool loop = true);
	~SpriteAnimationPlayer();
	void Init() override;
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
	void Update(float deltatime) override;
	void Set2DPosition(float x, float y);
	Vector2	Get2DPosition();
	void SetRotation(double angle);
	void SetFlip(SDL_RendererFlip flip);
	void SetSize(int width, int height);
	int	GetWidth();
	int	GetHeight();
	int	GetCurrentFrame() { return m_currentFrame; }
	int	GetStartFrame() { return m_frameStartIndex; }
	int	GetEndFrame() { return m_frameEndIndex; }
	void SetCurrentFrame(int frame) { m_currentFrame = frame; }
	void SetStartFrame(int startFrame) { m_frameStartIndex = startFrame; }
	void SetEndFrame(int endFrame) { m_frameEndIndex = endFrame; }
	float GetFrameTime() { return m_frameTime; }
	void SetFrameTime(float frameTime) { m_frameTime = frameTime; }
	void SetFrame(int startFrameIndex, int endFrameIndex);
	bool GetLoop() { return m_loop; }
	void SetLoop(bool loop) { m_loop = loop; }
};