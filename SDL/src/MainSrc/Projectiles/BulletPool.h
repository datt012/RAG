#pragma once
#include <vector>
#include <memory>
#include "Bullet.h"
#include "MainSrc/Map/Map.h"
#include "MainSrc/Collision/Collision.h"
class BulletPool {
public:
    BulletPool(int poolSize, std::shared_ptr<TextureManager> texture, SDL_RendererFlip flip);
    ~BulletPool();
    std::shared_ptr<Bullet> SpawnBullet(int damage);
    void Update(float deltaTime);
    void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
    void SolveCollision(std::shared_ptr<Map> map);
    const std::vector<std::shared_ptr<Bullet>>& GetBullets() const;
private:
    std::vector<std::shared_ptr<Bullet>>m_Bullets;
    std::shared_ptr<TextureManager>m_texture;
    SDL_RendererFlip m_flip;
};
