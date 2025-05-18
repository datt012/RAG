#include "Pixel.h"
Pixel::Pixel() {}
Pixel::~Pixel() {}
void Pixel::Init() {
	if (m_pTexture) {
		m_DestRect.w = m_SrcRect.w * m_scale.x;
		m_DestRect.h = m_SrcRect.h * m_scale.y;
	}
}
void Pixel::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    if (m_pTexture) {
        if (m_DestRect.x > SCREEN_WIDTH || m_DestRect.y > SCREEN_HEIDHT || m_DestRect.x + m_DestRect.w < 0 || m_DestRect.y + m_DestRect.h < 0) return;
        SDL_RenderCopy(renderer, m_pTexture->GetTextureObj(), &m_SrcRect, &m_DestRect);
    }
}
void Pixel::Update(float deltaTime) {
    m_DestRect.x = m_position.x - Camera::GetInstance()->GetPosition().x;
    m_DestRect.y = m_position.y - Camera::GetInstance()->GetPosition().y;
    m_DestRect.w = PIXEL_WIDTH;
    m_DestRect.h = PIXEL_HEIGHT;
}
SDL_Rect Pixel::GetDestRect() const {
    return m_DestRect;
}
void Pixel::SetSourceRect(const SDL_Rect& srcRect) {
    m_SrcRect = srcRect;
}
SDL_Rect Pixel::GetRectBox() {
    return {
        static_cast<int>(m_position.x),
        static_cast<int>(m_position.y),
        PIXEL_WIDTH,
        PIXEL_HEIGHT
    };
}
SDL_FRect Pixel::GetFRectBox() {
    return {
        m_position.x,
        m_position.y,
        PIXEL_WIDTH,
        PIXEL_HEIGHT
    };
}