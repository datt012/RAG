#pragma once
#include "BaseObject2d.h"
#include "TextureManager.h"

class Laser : public BaseObject2d {
public:
    Laser();
    ~Laser();

    void Init() override;
    void Init2(std::shared_ptr<TextureManager> bulletTexture, float speed);
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;

    void SetPath(std::string path) {
        m_filePath = path;
    }
    std::string GetPath() const {
        return m_filePath;
    }

    void SetSpeed(float spd) {
        m_speed = spd;
    }
    float GetSpeed() const {
        return m_speed;
    }
    void SetSize(float x, float y);

    SDL_Texture* GetLaserTexture() {
        return m_pTexture->GetTextureObj();
    }
    void SetDirection(Vector2 v) {
        m_direction = v;
    }
    void SetFlip(SDL_RendererFlip flip) {
        m_flip = flip;
    }
private:
    std::string m_filePath;
    float m_speed;
    Vector2 m_direction;
    
};
