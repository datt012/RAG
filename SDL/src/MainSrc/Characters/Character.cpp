#include "Character.h"
#include <MainSrc/Collision/Collision.h>

void Character::SetSize(int width, int height) { 
	m_animationPlayer->SetSize(width, height); 
}

int Character::GetWidth() { 
	return m_animationPlayer->GetWidth(); 
}

int Character::GetHeight() { 
	return m_animationPlayer->GetHeight(); 
}

Vector3 Character::GetPosition() {
	return m_animationPlayer->GetPosition();
}

void Character::SetPosition(Vector3 position) {
	Vector2 p = Get2DPosition();
	m_Displacement.x += position.x - p.x;
	m_Displacement.y += position.y - p.y;
	m_animationPlayer->SetPosition(position);
	m_position = position;
}

Vector2 Character::Get2DPosition() {
	return m_animationPlayer->Get2DPosition();
}

void Character::Set2DPosition(float x, float y) {
	Vector2 p = Get2DPosition();
	m_Displacement.x += x - p.x;
	m_Displacement.y += y - p.y;
	m_animationPlayer->Set2DPosition(x, y);
	m_position = Vector3((float)x, (float)y, 0.0f);
}

void Character::SetRotation(double angle) {
	m_animationPlayer->SetRotation(angle);
	m_angle = angle;
}

void Character::SetFlip(SDL_RendererFlip flip) {
	m_animationPlayer->SetFlip(flip);
	m_flip = flip;
}

void Character::SetVelocity(Vector2 velocity) {
	m_Velocity = velocity;
}

void Character::SetVelocity(float vx, float vy) {
	m_Velocity = { vx, vy };
}

Vector2 Character::GetVelocity() {
	return m_Velocity;
}

void Character::SetSpeed(float speed) {
	m_Speed = speed;
}

void Character::SetJumpForce(float force) {
	m_Speed = force;
}

void Character::SetJumpDuration(float duration) {
	m_Speed = duration;
}

void Character::SetGravity(float gravity) {
	m_Speed = gravity;
}

SDL_Rect Character::GetColliderRect() {
	return {
		static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		GetWidth(),
		GetWidth()
	};
}

SDL_FRect Character::GetColliderFRect() {
	return {
		m_position.x,
		m_position.y,
		static_cast<float>(GetWidth()),
		static_cast<float>(GetWidth())
	};
}

void Character::ClampPositionToMapBounds(std::shared_ptr<Map> map) {
	if (!map) return;

	SDL_FRect rect = GetColliderFRect();

	if (rect.y + rect.h >= map->GetHeight()) {
		m_HP = 0;
	}

	float left = rect.x - Get2DPosition().x;
	float right = (Get2DPosition().x + GetWidth()) - (rect.x + rect.w);
	float top = rect.y - Get2DPosition().y;
	float bottom = (Get2DPosition().y + GetHeight()) - (rect.y + rect.h);

	float x = std::min(std::max(0.0f - left, Get2DPosition().x), map->GetWidth() + right - GetWidth());
	float y = std::min(std::max(0.0f - top, Get2DPosition().y), map->GetHeight() + bottom - GetHeight());
	Set2DPosition(x, y);
}

void Character::UpdateIsOnGround(std::shared_ptr<Map> map) {
	if (!map) return;
	auto layer = map->GetCollisionLayer();
	if (!layer) return;

	m_IsOnGround = false;
	if (m_Velocity.y < 0) return;

	for (Pixel& pixel : layer->GetPixels()) {
		if (Collision::IsOnGround(GetColliderFRect(), pixel.GetFRectBox())) {
			m_IsOnGround = true;
			m_Velocity.y = 0;

			return;
		}
	}
}

bool Character::CollisionWithMap(std::shared_ptr<Map> map) {
	bool result = false;

	if (!map) return result;
	auto layer = map->GetCollisionLayer();
	if (!layer) return result;

	std::vector<SDL_FRect> obstacles = std::vector<SDL_FRect>();
	for (Pixel& pixel : layer->GetPixels()) {
		obstacles.push_back(pixel.GetFRectBox());
	}
	if (Collision::CheckAABB(GetColliderFRect(), obstacles)) {
		Vector2 push = Collision::GetPushVector(GetColliderFRect(), obstacles, m_Displacement);

		m_position.x += push.x;
		m_position.y += push.y;

		m_animationPlayer->Set2DPosition(m_position.x, m_position.y);
		result = true;
	}

	return result;
}

void Character::SolveCollision(std::shared_ptr<Map> map) {
	ClampPositionToMapBounds(map);
	CollisionWithMap(map);
	UpdateIsOnGround(map);
}

void Character::DrawCollider(SDL_Renderer* renderer, SDL_Color lineColor) {
	SDL_Rect colliderRect = GetColliderRect();

	colliderRect.x -= Camera::GetInstance()->GetPosition().x;
	colliderRect.y -= Camera::GetInstance()->GetPosition().y;

	SDL_SetRenderDrawColor(renderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	SDL_RenderDrawRect(renderer, &colliderRect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset color to default
}

void Character::DrawHPBar(SDL_Renderer* renderer, const Vector2& position, int currentHP, int maxHP,
							int barWidth, int barHeight, SDL_Color bgColor, SDL_Color hpColor, bool isWorldPosition) {
	float hpPercent = static_cast<float>(currentHP) / maxHP;
	if (hpPercent < 0) hpPercent = 0;
	if (hpPercent > 1) hpPercent = 1;

	Vector2 renderPos = position;
	if (isWorldPosition) {
		renderPos.x -= Camera::GetInstance()->GetPosition().x;
		renderPos.y -= Camera::GetInstance()->GetPosition().y;
	}

	SDL_Rect hpBarRect = { static_cast<int>(renderPos.x), static_cast<int>(renderPos.y), barWidth, barHeight };
	SDL_Rect hpFillRect = { static_cast<int>(renderPos.x), static_cast<int>(renderPos.y), barWidth * hpPercent, barHeight };

	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRect(renderer, &hpBarRect);

	SDL_SetRenderDrawColor(renderer, hpColor.r, hpColor.g, hpColor.b, hpColor.a);
	SDL_RenderFillRect(renderer, &hpFillRect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset color to default
}

// Getters
int Character::GetHP() const {
	return m_HP;
}

bool Character::IsAlive() const {
	return m_HP > 0;
}

int Character::GetMAXHP() const {
	return m_MAX_HP;
}

int Character::GetShootCooldown() const {
	return m_ShootCooldown;
}

int Character::GetShootCooldownTime() const {
	return m_ShootCooldownTime;
}

bool Character::IsOnGround() const {
	return m_IsOnGround;
}

bool Character::IsJumping() const {
	return m_IsJumping;
}

bool Character::IsShooting() const {
	return m_IsShooting;
}

std::shared_ptr<BulletPool> Character::GetBulletPool() const {
	return m_BulletPool;
}

// Setters
void Character::SetHP(int hp) {
	m_HP = hp;
	m_HP = std::max(0, std::min(m_HP, m_MAX_HP)); 
}

void Character::TakeDamage(int damage) {
	SetHP(m_HP - damage);
}

void Character::SetShootCooldown(int time) {
	m_ShootCooldown = time;
}

void Character::SetOnGround(bool isOnGround) {
	m_IsOnGround = isOnGround;
}

void Character::SetJumping(bool isJumping) {
	m_IsJumping = isJumping;
}

void Character::SetShooting(bool isShooting) {
	m_IsShooting = isShooting;
}