#include "Pixel.h"
#include "Define.h"
Pixel::Pixel( int pID, SDL_Texture* pTexture, SDL_Rect pRect) {
	pixelWidth = PIXEL_WIDTH;
	pixelHeight = PIXEL_HEIGHT;
	pixelID = pID;
	pixelTexture = pTexture;
	pixelSceenRect = pRect;
}
void Pixel::draw(SDL_Renderer* renderer, SDL_Rect srcRect) {
	if (renderer && pixelTexture) {
		SDL_RenderCopy(renderer, pixelTexture, &srcRect, &pixelSceenRect);
	}
}