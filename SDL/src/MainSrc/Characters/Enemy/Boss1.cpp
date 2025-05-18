#include "Boss1.h"
Boss1::Boss1(std::shared_ptr<SpriteAnimationPlayer> sprite) : Enemy(sprite) {}
Boss1::~Boss1() {}
void Boss1::Init() {
	m_MAX_HP = BOSS1_MAX_HP;
	m_HP = m_MAX_HP;
	m_Speed = BOSS1_SPEED_X;
	m_JumpForce = BOSS1_JUMP_FORCE;
	m_Gravity = GRAVITY;
	m_sightRange = BOSS1_SIGHT_RANGE;
	m_shootRange = BOSS1_SHOOT_RANGE;
	m_ShootCooldownTime = BOSS1_SHOOT_COOLDOWN_TIME;
	std::shared_ptr<TextureManager> texture = ResourceManagers::GetInstance()->GetTexture(BOSS1_BULLET_SPRITE_PATH);
	m_BulletPool = std::make_shared<BulletPool>(BOSS1_BULLET_POOL_SIZE, texture, SDL_FLIP_NONE);
}
void Boss1::HandleInput(int keyMask) {
	m_IsShooting = false;
	if (!IsAlive()) return;
	m_IsShooting = keyMask & KEY_SHOOT;
	m_CurrentDirectionGun = DirectionGun::FromKeyMask(keyMask);
	m_Velocity.x = m_CurrentDirectionGun & DirectionGun::LEFT ? -m_Speed : m_CurrentDirectionGun & DirectionGun::RIGHT ? m_Speed : 0;
}
void Boss1::Shoot() {
	std::shared_ptr<Bullet> bullet1;
	std::shared_ptr<Bullet> bullet2;
	std::shared_ptr<Bullet> bullet3;
	switch (m_CurrentDirectionGun) {
	case DirectionGun::NONE:
		bullet1 = m_BulletPool->SpawnBullet(BOSS1_BULLET_DAMAGE);
		bullet2 = m_BulletPool->SpawnBullet(BOSS1_BULLET_DAMAGE);
		bullet3 = m_BulletPool->SpawnBullet(BOSS1_BULLET_DAMAGE);
		break;
	case DirectionGun::LEFT:
		bullet2 = m_BulletPool->SpawnBullet(BOSS1_BULLET_DAMAGE);
		m_ShootCooldown /= 3;
		break;
	case DirectionGun::RIGHT:
		bullet3 = m_BulletPool->SpawnBullet(BOSS1_BULLET_DAMAGE);
		m_ShootCooldown /= 3;
		break;
	default:
		break;
	}
	Vector2 velocityBullet = { 0, 1 };
	velocityBullet = velocityBullet.Normalize() * BOSS1_BULLET_SPEED;
	Vector2 p1 = {
		Get2DPosition().x + GetWidth() / 2.0f,
		Get2DPosition().y + GetHeight() * 0.875f
	};
	Vector2 p2 = {
		Get2DPosition().x + GetWidth() * (0.5f - 0.17f),
		Get2DPosition().y + GetHeight() * (0.5f + 0.13f)
	};
	Vector2 p3 = {
		Get2DPosition().x + GetWidth() * (0.5f + 0.17f),
		Get2DPosition().y + GetHeight() * (0.5f + 0.13f)
	};
	if (bullet1) {
		bullet1->SetSize(BOSS1_BULLET_WIDTH, BOSS1_BULLET_HEIGHT);
		bullet1->Set2DPosition(p1.x, p1.y);
		bullet1->SetVelocity(velocityBullet * 1.05f);
	}
	if (bullet2) {
		bullet2->SetSize(BOSS1_BULLET_WIDTH, BOSS1_BULLET_HEIGHT);
		bullet2->Set2DPosition(p2.x, p2.y);
		bullet2->SetVelocity(velocityBullet);
	}
	if (bullet3) {
		bullet3->SetSize(BOSS1_BULLET_WIDTH, BOSS1_BULLET_HEIGHT);
		bullet3->Set2DPosition(p3.x, p3.y);
		bullet3->SetVelocity(velocityBullet);
	}
}
void Boss1::Update(float deltatime) {
	if (m_IsShooting && m_ShootCooldown <= 0) {
		m_ShootCooldown = m_ShootCooldownTime;
		Shoot();
	}
	else if (m_animationPlayer->GetCurrentFrame() == m_animationPlayer->GetEndFrame()) {
		m_IsShooting = false;
	}
	m_ShootCooldown -= (m_ShootCooldown > 0) ? static_cast<int>(deltatime) : 0;
	m_Velocity.x = m_Velocity.x * 0.98f;
	m_Velocity.y = m_Velocity.y * 0.98f;
	Vector2 position = Get2DPosition();
	position.x += m_Velocity.x * deltatime;
	position.y += m_Velocity.y * deltatime;
	Set2DPosition(position.x, position.y);
	m_animationPlayer->Update(deltatime);
}
void Boss1::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
	if (!IsAlive()) {
		m_animationPlayer->SetFrame(1, 6);
		m_animationPlayer->SetFrameTime(150);
		m_animationPlayer->SetLoop(false);
	}
	else {
		m_animationPlayer->SetFrame(0, 0);
		m_animationPlayer->SetLoop(false);
	}
	m_animationPlayer->Draw(renderer, clip);
	m_Displacement = { 0, 0 };
	SDL_FRect colliderRect = GetColliderFRect();
	if (IsAlive()) {
		DrawHPBar(renderer, { Get2DPosition().x, Get2DPosition().y - 3}, m_HP, m_MAX_HP, GetWidth(), 3);
	}
}
SDL_Rect Boss1::GetColliderRect() {
	SDL_FRect rect = GetColliderFRect();
	return {
		static_cast<int>(rect.x),
		static_cast<int>(rect.y),
		static_cast<int>(rect.w),
		static_cast<int>(rect.h)
	};
}
SDL_FRect Boss1::GetColliderFRect() {
	if (!IsAlive()) return {
		Get2DPosition().x,
		Get2DPosition().y + GetHeight() - 1,
		static_cast<float>(GetWidth()),1
	};
	SDL_FRect rect = {
		m_animationPlayer->Get2DPosition().x,
		m_animationPlayer->Get2DPosition().y,
		static_cast<float>(GetWidth()),
		static_cast<float>(GetHeight())
	};
	return  m_IsOnGround && m_CurrentDirectionGun == DirectionGun::DOWN ? SDL_FRect{
		rect.x + ORIGINAL_BOSS1_CROUCH_X * rect.w / ORIGINAL_BOSS1_SIZE_W,
		rect.y + ORIGINAL_BOSS1_CROUCH_Y * rect.h / ORIGINAL_BOSS1_SIZE_H,
		rect.w * ORIGINAL_BOSS1_CROUCH_W / ORIGINAL_BOSS1_SIZE_W,
		rect.h * ORIGINAL_BOSS1_CROUCH_H / ORIGINAL_BOSS1_SIZE_H
	} : SDL_FRect{
		rect.x + ORIGINAL_BOSS1_X * rect.w / ORIGINAL_BOSS1_SIZE_W,
		rect.y + ORIGINAL_BOSS1_Y * rect.h / ORIGINAL_BOSS1_SIZE_H,
		rect.w * ORIGINAL_BOSS1_W / ORIGINAL_BOSS1_SIZE_W,
		rect.h * ORIGINAL_BOSS1_H / ORIGINAL_BOSS1_SIZE_H
	};
}