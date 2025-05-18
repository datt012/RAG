#pragma once
#include <functional>
#include "Sprite2D.h"
class MouseButton : public Sprite2D {
public:
	MouseButton(std::shared_ptr<TextureManager> texture, SDL_RendererFlip flip);
	bool HandleTouchEvent(SDL_Event* e);
	void SetOnClick(std::function<void(void)>pBtClick);
private:
	std::function<void(void)>m_pBtClick;
};

