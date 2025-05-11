#include "Bullet.h"

void Bullet::Init()
{
	m_IsActive = true;
}

void Bullet::Update(float deltatime)
{
	m_position.x += m_Velocity.x * deltatime;
	m_position.y += m_Velocity.y * deltatime;

	Sprite2D::Update(deltatime);
}

void Bullet::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{
	if (m_pTexture != nullptr)
	{
		Vector2 camPosition = Camera::GetInstance()->GetPosition();
		SDL_Rect renderQuad = { m_position.x - camPosition.x, m_position.y - camPosition.y, m_iWidth, m_iHeight };
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}
		SDL_Point center = { renderQuad.w / 2, renderQuad.h / 2 };
		SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), m_pTexture->GetTextureObj(), clip, &renderQuad, m_angle, &center, m_flip);
	}
}

bool Bullet::IsActive() const
{
	return m_IsActive;
}

void Bullet::Deactivate()
{
	m_IsActive = false;
}

SDL_Rect Bullet::getColliderRect()
{
	SDL_FRect rect = getColliderFRect();
	return {
		static_cast<int>(rect.x),
		static_cast<int>(rect.y),
		static_cast<int>(rect.w),
		static_cast<int>(rect.h)
	};
}

SDL_FRect Bullet::getColliderFRect()
{
	SDL_FRect rect = {
		m_position.x,
		m_position.y,
		static_cast<float>(GetWidth()),
		static_cast<float>(GetHeight())
	};
	return rect;
}

// Getters
int Bullet::GetDamage() const
{
	return m_damage;
}

Vector2 Bullet::GetVelocity() const
{
	return m_Velocity;
}

// Setters
void Bullet::SetDamage(int damage)
{
	m_damage = damage;
}

void Bullet::SetVelocity(Vector2 velocity)
{
	m_Velocity = velocity;
}

void Bullet::SetVelocity(float vx, float vy)
{
	m_Velocity = { vx, vy };
}