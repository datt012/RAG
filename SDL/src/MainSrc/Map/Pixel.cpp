#include "Pixel.h"
#include "Define.h"
#include "iostream"
void Pixel::Init() {
	pixelWidth = PIXEL_WIDTH;
	pixelHeight = PIXEL_HEIGHT;
	pixelType = "g";// mặc định
	pixelSrcRect = { 0, 0, PIXEL_WIDTH, PIXEL_HEIGHT };
}
void Pixel::Update(float deltaTime) {
	//
}
void Pixel::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    if (m_pTexture == NULL) {
        return;
    }

    SDL_Texture* texture = m_pTexture->GetTextureObj();
    if (texture == NULL) {
        return;
    }

    SDL_Rect sceenRect;
    sceenRect.x = (int)m_position.x - Camera::GetInstance()->GetPosition().x;
    sceenRect.y = (int)m_position.y - Camera::GetInstance()->GetPosition().y;
    sceenRect.w = PIXEL_WIDTH;
    sceenRect.h = PIXEL_HEIGHT;

    if (SDL_RenderCopy(renderer, texture, &pixelSrcRect, &sceenRect) != 0) {
        std::cerr << "SDL_RenderCopy failed! Error: " << SDL_GetError() << std::endl;
    }
}