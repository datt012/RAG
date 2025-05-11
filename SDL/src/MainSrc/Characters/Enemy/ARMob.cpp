#include "ARMob.h"

const std::unordered_map<AnimationKey, std::pair<int, int>> ARMob::s_AnimationMap = {
	{{true, false, DirectionGun::NONE}, {0, 4}},
	{{true, false, DirectionGun::LEFT}, {13, 20}},
	{{true, false, DirectionGun::RIGHT}, {13, 20}},
	{{true, false, DirectionGun::UP}, {7, 7}},
	{{true, false, DirectionGun::DOWN}, {10, 10}},
	{{true, false, DirectionGun::LEFT_UP}, {56, 63}},		// NEED EDIT
	{{true, false, DirectionGun::RIGHT_UP}, {56, 63}},		// NEED EDIT
	{{true, false, DirectionGun::LEFT_DOWN}, {64, 71}},		// NEED EDIT
	{{true, false, DirectionGun::RIGHT_DOWN}, {64, 71}},	// NEED EDIT

	{{true, true, DirectionGun::NONE}, {5, 6}},
	{{true, true, DirectionGun::LEFT}, {24, 31}},
	{{true, true, DirectionGun::RIGHT}, {24, 31}},
	{{true, true, DirectionGun::UP}, {8, 9}},
	{{true, true, DirectionGun::DOWN}, {11, 12}},
	{{true, true, DirectionGun::LEFT_UP}, {72, 79}},		// NEED EDIT
	{{true, true, DirectionGun::RIGHT_UP}, {72, 79}},		// NEED EDIT
	{{true, true, DirectionGun::LEFT_DOWN}, {80, 87}},		// NEED EDIT
	{{true, true, DirectionGun::RIGHT_DOWN}, {80, 87}},		// NEED EDIT

	{{false, false, DirectionGun::NONE}, {15, 15}},
	{{false, false, DirectionGun::LEFT}, {15, 15}},
	{{false, false, DirectionGun::RIGHT}, {15, 15}},
	{{false, false, DirectionGun::UP}, {17, 17}},
	{{false, false, DirectionGun::DOWN}, {16, 16}},
	{{false, false, DirectionGun::LEFT_UP}, {51, 51}},		// NEED EDIT
	{{false, false, DirectionGun::RIGHT_UP}, {51, 51}},		// NEED EDIT
	{{false, false, DirectionGun::LEFT_DOWN}, {88, 88}},	// NEED EDIT
	{{false, false, DirectionGun::RIGHT_DOWN}, {88, 88}},	// NEED EDIT

	{{false, true, DirectionGun::NONE}, {40, 41}},
	{{false, true, DirectionGun::LEFT}, {40, 41}},
	{{false, true, DirectionGun::RIGHT}, {40, 41}},
	{{false, true, DirectionGun::UP}, {42, 43}},
	{{false, true, DirectionGun::DOWN}, {44, 45}},
	{{false, true, DirectionGun::LEFT_UP}, {52, 53}},		// NEED EDIT
	{{false, true, DirectionGun::RIGHT_UP}, {52, 53}},		// NEED EDIT
	{{false, true, DirectionGun::LEFT_DOWN}, {54, 55}},		// NEED EDIT
	{{false, true, DirectionGun::RIGHT_DOWN}, {54, 55}},	// NEED EDIT
	// ...
};

ARMob::ARMob(std::shared_ptr<SpriteAnimationPlayer> sprite) :
	Enemy(sprite)
{
}

ARMob::~ARMob()
{
}

void ARMob::Init()
{
	m_MAX_HP = ARMOB_MAX_HP;
	m_HP = m_MAX_HP;
	m_Speed = ARMOB_SPEED_X;
	m_JumpForce = ARMOB_JUMP_FORCE;
	m_Gravity = GRAVITY;
	m_sightRange = ARMOB_SIGHT_RANGE;
	m_shootRange = ARMOB_SHOOT_RANGE;

	m_ShootCooldownTime = ARMOB_SHOOT_COOLDOWN_TIME;

	std::shared_ptr<TextureManager> texture = ResourceManagers::GetInstance()->GetTexture(ARMOB_BULLET_SPRITE_PATH);
	m_BulletPool = std::make_shared<BulletPool>(ARMOB_BULLET_POOL_SIZE, texture, SDL_FLIP_NONE);
}

void ARMob::HandleInput(int keyMask)
{
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

void ARMob::Shoot()
{
	std::shared_ptr<Bullet> bullet = m_BulletPool->SpawnBullet(ARMOB_BULLET_DAMAGE);

	bool isCrouch = m_IsOnGround && m_CurrentDirectionGun == DirectionGun::DOWN;

	if (bullet) {
		bullet->SetSize(ARMOB_BULLET_WIDTH, ARMOB_BULLET_HEIGHT);

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
		velocityBullet = velocityBullet.Normalize() * ARMOB_BULLET_SPEED;

		Vector2 p;
		p.x = Get2DPosition().x +
			GetWidth() / 2.0f +
			(-bullet->GetWidth() / 2.0f);
		p.y = Get2DPosition().y + 
			static_cast<float>(GetHeight()) / ORIGINAL_ARMOB_SIZE_H * 20 +
			(-bullet->GetHeight() / 2.0f);
		p += velocityBullet / velocityBullet.Length() * 11 * GetWidth() / ORIGINAL_ARMOB_SIZE_W;

		bullet->Set2DPosition(p.x, p.y);
		bullet->SetVelocity(velocityBullet);
		bullet->SetRotation(atan2(velocityBullet.y, velocityBullet.x) * (180.0 / M_PI));
	}
}

void ARMob::Update(float deltatime)
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

void ARMob::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{

	AnimationKey key{ m_IsOnGround, m_IsShooting, m_CurrentDirectionGun };

	auto it = s_AnimationMap.find(key);
	if (!IsAlive())
	{
		m_animationPlayer->SetFrame(21, 24);
		m_animationPlayer->SetFrameTime(150);
		m_animationPlayer->SetLoop(false);
	}
	else if (it != s_AnimationMap.end())
	{
		m_animationPlayer->SetFrame(it->second.first, it->second.second);
	}

	m_animationPlayer->Draw(renderer, clip);

	m_Displacement = { 0, 0 };

	SDL_FRect colliderRect = GetColliderFRect();
	//DrawCollider(renderer);
	if (IsAlive()) {
		DrawHPBar(renderer, { colliderRect.x, colliderRect.y - 5 }, m_HP, m_MAX_HP, colliderRect.w, 3);
	}
}

SDL_Rect ARMob::GetColliderRect() {
	SDL_FRect rect = GetColliderFRect();

	return {
		static_cast<int>(rect.x),
		static_cast<int>(rect.y),
		static_cast<int>(rect.w),
		static_cast<int>(rect.h)
	};
}

SDL_FRect ARMob::GetColliderFRect() {
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
		rect.x + ORIGINAL_ARMOB_CROUCH_X * rect.w / ORIGINAL_ARMOB_SIZE_W,
		rect.y + ORIGINAL_ARMOB_CROUCH_Y * rect.h / ORIGINAL_ARMOB_SIZE_H,
		rect.w * ORIGINAL_ARMOB_CROUCH_W / ORIGINAL_ARMOB_SIZE_W,
		rect.h * ORIGINAL_ARMOB_CROUCH_H / ORIGINAL_ARMOB_SIZE_H
	} : SDL_FRect{
		rect.x + ORIGINAL_ARMOB_X * rect.w / ORIGINAL_ARMOB_SIZE_W,
		rect.y + ORIGINAL_ARMOB_Y * rect.h / ORIGINAL_ARMOB_SIZE_H,
		rect.w * ORIGINAL_ARMOB_W / ORIGINAL_ARMOB_SIZE_W,
		rect.h * ORIGINAL_ARMOB_H / ORIGINAL_ARMOB_SIZE_H
	};
}