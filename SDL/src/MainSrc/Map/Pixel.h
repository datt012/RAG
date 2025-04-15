#pragma once
#include "SDL.h"
using namespace std;
class Pixel {
public:
	int pixelWidth;
	int pixelHeight;
	int pixelID; // id của pixel trong tập tileset
	SDL_Texture* pixelTexture;
	SDL_Rect pixelSceenRect;

	Pixel(int pID, SDL_Texture* pTexture, SDL_Rect pRect);
	void draw(SDL_Renderer* renderer, SDL_Rect srcRect);
};
