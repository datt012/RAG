#pragma once
#include "GameObject/BaseObject2d.h"
#include "SpriteAnimation.h"

class ExplosiveBullet : public BaseObject2d
{
private:
    float m_speed;
    bool m_isExploded;
    bool m_flag;// cờ đánh dấu viện đạn xóa hay k
    std::shared_ptr<SpriteAnimation> m_explosionAnimation; 

public:
    ExplosiveBullet();
    ~ExplosiveBullet();

    void Init2(std::shared_ptr<TextureManager> texture);
    void Init() override;
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;

    void SetSpeed(float speed) {
        m_speed = speed;
    }
    void SetFlag(bool value) {
        m_flag = value;
    }  
};
