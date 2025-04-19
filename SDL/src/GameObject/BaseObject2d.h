#pragma once
#include "CMath.h"
#include <memory>
#include "SDL.h"

#include "TextureManager.h"
class BaseObject2d
{
protected:
	Vector2 m_position;
	Vector2 m_scale;
	double m_angle = 0.0; // default angle
	SDL_RendererFlip m_flip = SDL_FLIP_NONE;
	std::shared_ptr<TextureManager> m_pTexture;
public:
	BaseObject2d() : m_pTexture(nullptr), m_position(Vector2(0.0f, 0.0f)), m_scale(Vector2(1.0f, 1.0f)) {}
	BaseObject2d(std::shared_ptr<TextureManager> texture)
		:m_pTexture(texture), m_position(Vector2(0.0f, 0.0f)), m_scale(Vector2( 1.0f, 1.0f)) {
	}
	virtual void Init() = 0;
	virtual void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) = 0;
	virtual void Update(float deltatime) = 0;
	void SetPosition(Vector2 position) { m_position = position; }
	Vector2 GetPosition() { return m_position; }
	void SetTexture(std::shared_ptr<TextureManager> texture) { m_pTexture = texture; }

};

