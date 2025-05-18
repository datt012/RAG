#include "MouseButton.h"
MouseButton::MouseButton(std::shared_ptr<TextureManager> texture, SDL_RendererFlip flip) : Sprite2D(texture,flip), m_pBtClick(nullptr) {}
bool MouseButton::HandleTouchEvent(SDL_Event* e) {
	if (e->type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		if (x < m_position.x || x > m_position.x + m_iWidth|| y < m_position.y || y > m_position.y + m_iHeight) {
			inside = false;
		}
		if (inside) {
			m_pBtClick();
		}
		return inside;
	}
	return false;
}
void MouseButton::SetOnClick(std::function<void(void)> pBtClick) {
	m_pBtClick = pBtClick;
}





