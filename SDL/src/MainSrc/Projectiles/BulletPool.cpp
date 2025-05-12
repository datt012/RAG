#include "BulletPool.h"

BulletPool::BulletPool(int poolSize, std::shared_ptr<TextureManager> texture, SDL_RendererFlip flip)
	: m_texture(texture), m_flip(flip) 
{
    m_Bullets.reserve(poolSize);
    for (int i = 0; i < poolSize; ++i) {
        m_Bullets.push_back(std::make_shared<Bullet>(m_texture, m_flip));
    }
}

BulletPool::~BulletPool() {}

std::shared_ptr<Bullet> BulletPool::SpawnBullet(int damage) {
    for (auto& bullet : m_Bullets) {
        if (!bullet->IsActive()) {
            bullet->Init();
			bullet->SetDamage(damage);
            return bullet;
        }
    }
    return nullptr;
}

void BulletPool::Update(float deltaTime) {
    for (auto& bullet : m_Bullets) {
        if (bullet->IsActive()) {
            bullet->Update(deltaTime);
        }
    }
}

void BulletPool::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    for (auto& bullet : m_Bullets) {
        if (bullet->IsActive()) {
            bullet->Draw(renderer, clip);
        }
    }
}

void BulletPool::SolveCollision(std::shared_ptr<Map> map) {
	if (!map) return;
	auto layer = map->GetCollisionLayer();
	if (!layer) return;

    std::vector<SDL_FRect> obstacles = std::vector<SDL_FRect>();
    for (Pixel& pixel : layer->GetPixels()) {
        obstacles.push_back(pixel.GetFRectBox());
    }

	for (auto& bullet : m_Bullets) {
		if (bullet->IsActive()) {
            SDL_FRect rectBullet = bullet->getColliderFRect();

            if (Collision::CheckAABB(rectBullet, obstacles)) {
				bullet->Deactivate();
            }
            SDL_FRect rectMap = {
                0,
                0,
                static_cast<float>(map->GetWidth()),
                static_cast<float>(map->GetHeight())
            };

			if (!Collision::CheckAABB(rectBullet, rectMap)) {
				bullet->Deactivate();
			}
		}
	}
}

const std::vector<std::shared_ptr<Bullet>>& BulletPool::GetBullets() const {
    return m_Bullets;
}
