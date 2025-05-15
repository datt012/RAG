#include "Player.h"
#include "Sound.h"
#include "Define.h"

const std::unordered_map<AnimationKey, std::pair<int, int>> Player::s_AnimationMap = {
	{{true, false, DirectionGun::NONE}, {0, 5}},
	{{true, false, DirectionGun::LEFT}, {6, 13}},
	{{true, false, DirectionGun::RIGHT}, {6, 13}},
	{{true, false, DirectionGun::UP}, {21, 21}},
	{{true, false, DirectionGun::DOWN}, {14, 14}},
	{{true, false, DirectionGun::LEFT_UP}, {56, 63}},		// NEED EDIT
	{{true, false, DirectionGun::RIGHT_UP}, {56, 63}},		// NEED EDIT
	{{true, false, DirectionGun::LEFT_DOWN}, {64, 71}},		// NEED EDIT
	{{true, false, DirectionGun::RIGHT_DOWN}, {64, 71}},	// NEED EDIT

	{{true, true, DirectionGun::NONE}, {18, 19}},
	{{true, true, DirectionGun::LEFT}, {24, 31}},
	{{true, true, DirectionGun::RIGHT}, {24, 31}},
	{{true, true, DirectionGun::UP}, {20, 21}},
	{{true, true, DirectionGun::DOWN}, {22, 23}},
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

Player::Player(std::shared_ptr<SpriteAnimationPlayer> sprite) :
	Character(sprite)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_MAX_HP = PLAYER_MAX_HP;
	m_HP = m_MAX_HP;
	m_Speed = PLAYER_SPEED_X;
	m_JumpForce = PLAYER_JUMP_FORCE;
	m_Gravity = GRAVITY;

	m_ShootCooldownTime = PLAYER_SHOOT_COOLDOWN_TIME;

	std::shared_ptr<TextureManager> texture = ResourceManagers::GetInstance()->GetTexture(PLAYER_BULLET_SPRITE_PATH);
	m_BulletPool = std::make_shared<BulletPool>(PLAYER_BULLET_POOL_SIZE, texture, SDL_FLIP_NONE);
}

void Player::HandleInput(int keyMask)
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

void Player::Shoot()
{
	std::shared_ptr<Bullet> bullet = m_BulletPool->SpawnBullet(PLAYER_BULLET_DAMAGE);

	if (bullet) {
		bullet->SetSize(PLAYER_BULLET_WIDTH, PLAYER_BULLET_HEIGHT);

		bool isCrouch = m_IsOnGround && m_CurrentDirectionGun == DirectionGun::DOWN;

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
		velocityBullet = velocityBullet.Normalize() * PLAYER_BULLET_SPEED;

		Vector2 p;
		p.x = Get2DPosition().x +
			GetWidth() / 2.0f +
			(- bullet->GetWidth() / 2.0f);
		p.y = Get2DPosition().y + 
			static_cast<float>(GetHeight()) / ORIGINAL_PLAYER_SIZE_H * (isCrouch ? 32 : 26) +
			(-bullet->GetHeight() / 2.0f);
		p += velocityBullet / velocityBullet.Length() * 10 * GetWidth() / ORIGINAL_PLAYER_SIZE_W;

		bullet->Set2DPosition(p.x, p.y);
		bullet->SetVelocity(velocityBullet);
		bullet->SetRotation(atan2(velocityBullet.y, velocityBullet.x) * (180.0 / M_PI));

		// sfx
		Sound::GetInstance()->LoadSfx(Player_Sound);
		Sound::GetInstance()->PlaySfx(Player_Sound);
		
	}
}

void Player::Update(float deltatime)
{
	if (m_IsShooting && m_ShootCooldown <= 0) {
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

void Player::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{
	AnimationKey key{ m_IsOnGround, m_IsShooting, m_CurrentDirectionGun };

	auto it = s_AnimationMap.find(key);
	if (!IsAlive()) 
	{
		m_animationPlayer->SetFrame(46, 50);
		m_animationPlayer->SetFrameTime(150);
		m_animationPlayer->SetLoop(false);
	}
	else if (it != s_AnimationMap.end())
	{
		m_animationPlayer->SetFrame(it->second.first, it->second.second);
		m_animationPlayer->SetLoop(true);
	}

	m_animationPlayer->Draw(renderer, clip);

	m_Displacement = { 0, 0 };

	SDL_FRect colliderRect = GetColliderFRect();
	//DrawCollider(renderer);
	if (IsAlive()) {
		DrawHPBar(renderer, { colliderRect.x, Get2DPosition().y + static_cast<float>(GetHeight()) * ORIGINAL_PLAYER_Y / ORIGINAL_PLAYER_SIZE_H - 6},
			m_HP, m_MAX_HP, colliderRect.w, 3,
			{ 100, 100, 100, 255 }, { 0, 255, 0, 255 });
	}
}

SDL_Rect Player::GetColliderRect() {
	SDL_FRect rect = GetColliderFRect();

	return {
		static_cast<int>(rect.x),
		static_cast<int>(rect.y),
		static_cast<int>(rect.w),
		static_cast<int>(rect.h)
	};
}

SDL_FRect Player::GetColliderFRect() {
	if (!IsAlive()) return { 
		Get2DPosition().x, 
		Get2DPosition().y + GetHeight(),
		static_cast<float>(GetWidth()),
		0
	};

	SDL_FRect rect = {
		Get2DPosition().x,
		Get2DPosition().y,
		static_cast<float>(GetWidth()),
		static_cast<float>(GetHeight())
	};

	return  m_IsOnGround && m_CurrentDirectionGun == DirectionGun::DOWN ? SDL_FRect{
		rect.x + ORIGINAL_PLAYER_CROUCH_X * rect.w / ORIGINAL_PLAYER_SIZE_W,
		rect.y + ORIGINAL_PLAYER_CROUCH_Y * rect.h / ORIGINAL_PLAYER_SIZE_H,
		rect.w * ORIGINAL_PLAYER_CROUCH_W / ORIGINAL_PLAYER_SIZE_W,
		rect.h * ORIGINAL_PLAYER_CROUCH_H / ORIGINAL_PLAYER_SIZE_H
	} : SDL_FRect{
		rect.x + ORIGINAL_PLAYER_X * rect.w / ORIGINAL_PLAYER_SIZE_W,
		rect.y + ORIGINAL_PLAYER_Y * rect.h / ORIGINAL_PLAYER_SIZE_H,
		rect.w * ORIGINAL_PLAYER_W / ORIGINAL_PLAYER_SIZE_W,
		rect.h * ORIGINAL_PLAYER_H / ORIGINAL_PLAYER_SIZE_H
	};
}