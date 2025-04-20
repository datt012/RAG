#pragma once
#include "SDL.h"
#include "GameObject/BaseObject.h"
#include "GameObject/BaseObject2d.h"
#include "Camera.h"
#include <string>
class Pixel : public BaseObject2d {
public:
	int pixelWidth;
	int pixelHeight;
	SDL_Rect pixelSrcRect;
	std::string pixelType; // grall,wall,...

	void Init() override;
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
};