#pragma once

#include "BaseObject.h"
#include <SDL_render.h>
#include <memory>

class TextureManager;

class SpriteEnemy : public BaseObject {
protected:
    int m_iWidth;
    int m_iHeight;
    int m_frameCount;        // Tổng số frame trong texture
    int m_currentFrame;
    int m_startFrame;        // Frame bắt đầu animation
    int m_endFrame;          // Frame kết thúc animation
    float m_frameTime;       // Thời gian mỗi frame
    float m_currentTicks;    // Thời gian tích lũy để chuyển frame

public:
    SpriteEnemy(std::shared_ptr<TextureManager> texture, int frameCount, float frameTime);
    ~SpriteEnemy();

    void Init() override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = nullptr) override;
    void Update(float deltaTime) override;

    // Các Setter và Getter
    void Set2DPosition(float x, float y);
    void SetRotation(double angle);
    void SetFlip(SDL_RendererFlip flip);
    Vector2 Get2DPosition() const;
    void SetSize(int width, int height);
    int GetWidth() const;
    int GetHeight() const;

    // Animation control
    void SetFrameRange(int startFrame, int endFrame);
    bool IsAnimationFinished() const;
};
