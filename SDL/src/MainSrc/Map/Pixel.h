#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include "Define.h"
#include "Camera.h"
#include "BaseObject.h"

class Pixel : public BaseObject {
public:
    Pixel();
    ~Pixel();

    void SetSourceRect(const SDL_Rect& srcRect);

    void Init() override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
    void Update(float deltaTime) override;

private:
    SDL_Rect m_SrcRect;
    SDL_Rect m_DestRect;
};
