#include "Layer.h"

Layer::Layer(int width, int height) : m_Width(width), m_Height(height) {}

Layer::~Layer() {}

void Layer::Init() {
	for (auto& pixel : m_Pixels) {
		pixel.Init();
	}
}

void Layer::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
   if (!renderer || !visible) {
       return;
   }

   for (auto& pixel : m_Pixels) {
       if (!pixel.GetTexture()) {
           continue;
       }

       pixel.Draw(renderer);
   }
}

void Layer::Update(float deltaTime) {
   for (auto& pixel : m_Pixels) {
       pixel.Update(deltaTime);
   }
}

void Layer::AddPixel(const Pixel& pixel) {
    m_Pixels.push_back(pixel);
}

void Layer::ClearPixels() {
    m_Pixels.clear();
}