#pragma once
#include <vector>
#include "Pixel.h"
#include "BaseObject.h"

class Layer : public BaseObject {
public:
    Layer(int width, int height);
    ~Layer();

    void AddPixel(const Pixel& pixel);

	void Init() override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
    void Update(float deltaTime) override;

private:
    int m_Width;
    int m_Height;
    std::vector<Pixel> m_Pixels;
};
