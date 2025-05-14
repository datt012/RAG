#include "SniperMob.h"

const std::unordered_map<AnimationKey, std::pair<int, int>> SniperMob::s_AnimationMap = {
	{{true, false, DirectionGun::NONE}, {0, 0}},
	{{true, false, DirectionGun::LEFT}, {0, 0}},
	{{true, false, DirectionGun::RIGHT}, {0, 0}},
	{{true, false, DirectionGun::UP}, {0, 0}},
	{{true, false, DirectionGun::DOWN}, {0, 0}},
	{{true, false, DirectionGun::LEFT_UP}, {0, 0}},		// NEED EDIT
	{{true, false, DirectionGun::RIGHT_UP}, {0, 0}},		// NEED EDIT
	{{true, false, DirectionGun::LEFT_DOWN}, {0, 0}},		// NEED EDIT
	{{true, false, DirectionGun::RIGHT_DOWN}, {0, 0}},	// NEED EDIT

	{{true, true, DirectionGun::NONE}, {1, 8}},
	{{true, true, DirectionGun::LEFT}, {1, 8}},
	{{true, true, DirectionGun::RIGHT}, {1, 8}},
	{{true, true, DirectionGun::UP}, {1, 8}},
	{{true, true, DirectionGun::DOWN}, {1, 8}},
	{{true, true, DirectionGun::LEFT_UP}, {1, 8}},		// NEED EDIT
	{{true, true, DirectionGun::RIGHT_UP}, {1, 8}},		// NEED EDIT
	{{true, true, DirectionGun::LEFT_DOWN}, {1, 8}},		// NEED EDIT
	{{true, true, DirectionGun::RIGHT_DOWN}, {1, 8}},		// NEED EDIT

	{{false, false, DirectionGun::NONE}, {0, 0}},
	{{false, false, DirectionGun::LEFT}, {0, 0}},
	{{false, false, DirectionGun::RIGHT}, {0, 0}},
	{{false, false, DirectionGun::UP}, {0, 0}},
	{{false, false, DirectionGun::DOWN}, {0, 0}},
	{{false, false, DirectionGun::LEFT_UP}, {0, 0}},		// NEED EDIT
	{{false, false, DirectionGun::RIGHT_UP}, {0, 0}},		// NEED EDIT
	{{false, false, DirectionGun::LEFT_DOWN}, {0, 0}},	// NEED EDIT
	{{false, false, DirectionGun::RIGHT_DOWN}, {0, 0}},	// NEED EDIT

	{{false, true, DirectionGun::NONE}, {1, 8}},
	{{false, true, DirectionGun::LEFT}, {1, 8}},
	{{false, true, DirectionGun::RIGHT}, {1, 8}},
	{{false, true, DirectionGun::UP}, {1, 8}},
	{{false, true, DirectionGun::DOWN}, {1, 8}},
	{{false, true, DirectionGun::LEFT_UP}, {1, 8}},		// NEED EDIT
	{{false, true, DirectionGun::RIGHT_UP}, {1, 8}},		// NEED EDIT
	{{false, true, DirectionGun::LEFT_DOWN}, {1, 8}},		// NEED EDIT
	{{false, true, DirectionGun::RIGHT_DOWN}, {1, 8}},	// NEED EDIT
	// ...
};

SniperMob::SniperMob(std::shared_ptr<SpriteAnimationPlayer> sprite) :
	Enemy(sprite)
{
}

SniperMob::~SniperMob()
{
}

void SniperMob::Init()
{
	m_MAX_HP = SNIPER_MAX_HP;
	m_HP = m_MAX_HP;
	m_Speed = SNIPER_SPEED_X;
	m_JumpForce = SNIPER_JUMP_FORCE;
	m_Gravity = GRAVITY;
	m_sightRange = SNIPER_SIGHT_RANGE;
	m_shootRange = SNIPER_SHOOT_RANGE;

	m_ShootCooldownTime = SNIPER_SHOOT_COOLDOWN_TIME;

	std::shared_ptr<TextureManager> texture = ResourceManagers::GetInstance()->GetTexture(SNIPER_BULLET_SPRITE_PATH);
	m_BulletPool = std::make_shared<BulletPool>(SNIPER_BULLET_POOL_SIZE, texture, SDL_FLIP_NONE);
}

void SniperMob::HandleInput(int keyMask)
{
	keyMask |= KEY_DOWN;

	m_IsShooting = false;
	if (!IsAlive()) return;

	m_IsShooting = keyMask & KEY_SHOOT;
	m_CurrentDirectionGun = DirectionGun::FromKeyMask(keyMask);

	SetFlip(
		m_CurrentDirectionGun & DirectionGun::LEFT ? SDL_FLIP_HORIZONTAL :
		m_CurrentDirectionGun & DirectionGun::RIGHT ? SDL_FLIP_NONE : m_flip
	);

	m_Velocity.x =
		m_CurrentDirectionGun & DirectionGun::LEFT ? -m_Speed :
		m_CurrentDirectionGun & DirectionGun::RIGHT ? m_Speed : 0;

	if ((keyMask & KEY_JUMP) && m_IsOnGround)
	{
		m_Velocity.y = -m_JumpForce;
	}
}

void SniperMob::Shoot()
{
	std::shared_ptr<Bullet> bullet = m_BulletPool->SpawnBullet(SNIPER_BULLET_DAMAGE);

	bool isCrouch = m_IsOnGround && m_CurrentDirectionGun == DirectionGun::DOWN;

	if (bullet) {
		bullet->SetSize(SNIPER_BULLET_WIDTH, SNIPER_BULLET_HEIGHT);

		Vector2 velocityBullet = {
			m_CurrentDirectionGun & DirectionGun::LEFT ? -1.0f : m_CurrentDirectionGun & DirectionGun::RIGHT ? 1.0f : 0,
			m_CurrentDirectionGun & DirectionGun::UP ? -1.0f : m_CurrentDirectionGun & DirectionGun::DOWN ? 1.0f : 0
		};
		if (m_CurrentDirectionGun == DirectionGun::NONE) {
			velocityBullet.x = m_flip == SDL_FLIP_HORIZONTAL ? -1.0f : 1.0f;
		}
		if (isCrouch) {
			velocityBullet.x = m_flip == SDL_FLIP_HORIZONTAL ? -1.0f : 1.0f;
			velocityBullet.y = 0;
		}
		velocityBullet = velocityBullet.Normalize() * SNIPER_BULLET_SPEED;

		Vector2 p;
		p.x = Get2DPosition().x +
			GetWidth() / 2.0f +
			(-bullet->GetWidth() / 2.0f);
		p.y = Get2DPosition().y +
			static_cast<float>(GetHeight()) / ORIGINAL_SNIPER_SIZE_H * 28 +
			(-bullet->GetHeight() / 2.0f);
		p += velocityBullet / velocityBullet.Length() * 12 * GetWidth() / ORIGINAL_SNIPER_SIZE_W;

		bullet->Set2DPosition(p.x, p.y);
		bullet->SetVelocity(velocityBullet);
		bullet->SetRotation(atan2(velocityBullet.y, velocityBullet.x) * (180.0 / M_PI));
	}

	m_animationPlayer->SetCurrentFrame(m_animationPlayer->GetStartFrame());
}

void SniperMob::Update(float deltatime)
{
	if (m_IsShooting && m_ShootCooldown <= 0 && m_IsOnGround) {
		Shoot();
		m_ShootCooldown = m_ShootCooldownTime;
	}
	else if (m_animationPlayer->GetCurrentFrame() == m_animationPlayer->GetEndFrame()) {
		m_IsShooting = false;
	}
	m_ShootCooldown -= (m_ShootCooldown > 0) ? static_cast<int>(deltatime) : 0;

	m_Velocity.y += m_IsOnGround ? 0 : (m_Gravity * deltatime);

	m_Velocity.x = m_Velocity.x * 0.98f;
	m_Velocity.y = m_Velocity.y * 0.98f;

	Vector2 position = Get2DPosition();
	position.x += m_Velocity.x * deltatime;
	position.y += m_Velocity.y * deltatime;
	Set2DPosition(position.x, position.y);

	m_animationPlayer->Update(deltatime);
}

void SniperMob::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{

	AnimationKey key{ m_IsOnGround, m_IsShooting, m_CurrentDirectionGun };

	auto it = s_AnimationMap.find(key);
	if (!IsAlive())
	{
		m_animationPlayer->SetFrame(9, 12);
		m_animationPlayer->SetFrameTime(150);
		m_animationPlayer->SetLoop(false);
	}
	else if (it != s_AnimationMap.end())
	{
		m_animationPlayer->SetFrame(it->second.first, it->second.second);
		m_animationPlayer->SetLoop(false);
	}

	m_animationPlayer->Draw(renderer, clip);

	m_Displacement = { 0, 0 };

	SDL_FRect colliderRect = GetColliderFRect();
	//DrawCollider(renderer);
	if (IsAlive()) {
		DrawHPBar(renderer, { colliderRect.x, Get2DPosition().y + static_cast<float>(GetHeight()) * ORIGINAL_ARMOB_Y / ORIGINAL_ARMOB_SIZE_H - 6 }, m_HP, m_MAX_HP, colliderRect.w, 3);
	}
}

SDL_Rect SniperMob::GetColliderRect() {
	SDL_FRect rect = GetColliderFRect();

	return {
		static_cast<int>(rect.x),
		static_cast<int>(rect.y),
		static_cast<int>(rect.w),
		static_cast<int>(rect.h)
	};
}

SDL_FRect SniperMob::GetColliderFRect() {
	if (!IsAlive()) return {
		Get2DPosition().x,
		Get2DPosition().y + GetHeight(),
		static_cast<float>(GetWidth()),
		0
	};

	SDL_FRect rect = {
		m_animationPlayer->Get2DPosition().x,
		m_animationPlayer->Get2DPosition().y,
		static_cast<float>(GetWidth()),
		static_cast<float>(GetHeight())
	};

	return  m_IsOnGround && m_CurrentDirectionGun == DirectionGun::DOWN ? SDL_FRect{
		rect.x + ORIGINAL_SNIPER_CROUCH_X * rect.w / ORIGINAL_SNIPER_SIZE_W,
		rect.y + ORIGINAL_SNIPER_CROUCH_Y * rect.h / ORIGINAL_SNIPER_SIZE_H,
		rect.w * ORIGINAL_SNIPER_CROUCH_W / ORIGINAL_SNIPER_SIZE_W,
		rect.h * ORIGINAL_SNIPER_CROUCH_H / ORIGINAL_SNIPER_SIZE_H
	} : SDL_FRect{
		rect.x + ORIGINAL_SNIPER_X * rect.w / ORIGINAL_SNIPER_SIZE_W,
		rect.y + ORIGINAL_SNIPER_Y * rect.h / ORIGINAL_SNIPER_SIZE_H,
		rect.w * ORIGINAL_SNIPER_W / ORIGINAL_SNIPER_SIZE_W,
		rect.h * ORIGINAL_SNIPER_H / ORIGINAL_SNIPER_SIZE_H
	};
}