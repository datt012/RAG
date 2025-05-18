#pragma once
#include <vector>
#include "Pixel.h"
#include "BaseObject.h"
class Layer : public BaseObject {
public:
    Layer(int width, int height);
    ~Layer();
	void Init() override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
    void Update(float deltaTime) override;
	int GetID() const { return id; }
	const std::string& GetName() const { return name; }
    int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
	std::vector<Pixel>& GetPixels() { return m_Pixels; }
	bool IsVisible() const { return visible; }
	void SetID(int id) { this->id = id; }
	void SetName(const std::string& name) { this->name = name; }
	void SetWidth(int width) { m_Width = width; }
	void SetHeight(int height) { m_Height = height; }
	void SetVisible(bool visible) { this->visible = visible; }
	void AddPixel(const Pixel& pixel);
	void ClearPixels();
private:
    int id;
	std::string name;
    int m_Width;
    int m_Height;
    std::vector<Pixel> m_Pixels;
    bool visible;
};
