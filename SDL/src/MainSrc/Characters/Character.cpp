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

	float x = std::min(std::max(0.0f, m_animationPlayer->Get2DPosition().x), map->GetWidth() - GetWidth() * 1.0f);
	float y = std::min(std::max(0.0f, m_animationPlayer->Get2DPosition().y), map->GetHeight() - GetHeight() * 1.0f);
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

void Character::CollisionWithMap(std::shared_ptr<Map> map) {
	if (!map) return;
	auto layer = map->GetCollisionLayer();
	if (!layer) return;

	std::vector<SDL_FRect> obstacles = std::vector<SDL_FRect>();
	for (Pixel& pixel : layer->GetPixels()) {
		obstacles.push_back(pixel.GetFRectBox());
	}
	if (Collision::CheckAABB(GetColliderFRect(), obstacles)) {
		Vector2 push = Collision::GetPushVector(GetColliderFRect(), obstacles, m_Displacement);

		m_position.x += push.x;
		m_position.y += push.y;

		m_animationPlayer->Set2DPosition(m_position.x, m_position.y);
	}
}

void Character::SolveCollision(std::shared_ptr<Map> map) {
	ClampPositionToMapBounds(map);
	CollisionWithMap(map);
	UpdateIsOnGround(map);
}