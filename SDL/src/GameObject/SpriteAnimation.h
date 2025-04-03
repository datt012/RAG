#pragma once
#include "BaseObject.h"
#include <SDL_render.h>
class TextureManager;
class SpriteAnimation : public BaseObject{
protected:
	int			m_iHeight;
	int			m_iWidth;
	float			m_numFrames;
	int			m_currentFrame;
	float		m_frameTime;
	float		m_currentTime;
	int			m_currentAction; //start 1,2,3
	int		m_frameCount; // start from 1,2,3...
	float  m_currentTicks;
	Uint32 m_lastUpdate;
	int			m_animSpeed;
	int m_numAction; //start from 1,2,3...

public:
	
	SpriteAnimation(std::shared_ptr<TextureManager> texture, int currentAction, int frameCount, int numAction, float  frameTime);
	~SpriteAnimation();

	void		Init() override;
	void		Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
	void		Update(float deltatime) override;

	void		Set2DPosition(float x, float y);
	void		SetRotation(double angle);
	void SetFlip (SDL_RendererFlip flip);
	int GetCurrentAction();
	void SetCurrentAction(int action);//action start from 1
	Vector2		Get2DPosition();
	void		SetSize(int width, int height);
	int GetWidth();
	int GetHeight();
};