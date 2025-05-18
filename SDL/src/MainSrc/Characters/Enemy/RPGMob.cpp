#include "RPGMob.h"
#include "Sound.h"
const std::unordered_map<AnimationKey, std::pair<int, int>> RPGMob::s_AnimationMap = {
	{{true, false, DirectionGun::NONE}, {0, 0}},
	{{true, false, DirectionGun::LEFT}, {0, 0}},
	{{true, false, DirectionGun::RIGHT}, {0, 0}},
	{{true, false, DirectionGun::UP}, {0, 0}},
	{{true, false, DirectionGun::DOWN}, {0, 0}},
	{{true, false, DirectionGun::LEFT_UP}, {0, 0}},		
	{{true, false, DirectionGun::RIGHT_UP}, {0, 0}},		
	{{true, false, DirectionGun::LEFT_DOWN}, {0, 0}},		
	{{true, false, DirectionGun::RIGHT_DOWN}, {0, 0}},	
	{{true, true, DirectionGun::NONE}, {1, 5}},
	{{true, true, DirectionGun::LEFT}, {1, 5}},
	{{true, true, DirectionGun::RIGHT}, {1, 5}},
	{{true, true, DirectionGun::UP}, {1, 5}},
	{{true, true, DirectionGun::DOWN}, {1, 5}},
	{{true, true, DirectionGun::LEFT_UP}, {1, 5}},		
	{{true, true, DirectionGun::RIGHT_UP}, {1, 5}},		
	{{true, true, DirectionGun::LEFT_DOWN}, {1, 5}},		
	{{true, true, DirectionGun::RIGHT_DOWN}, {1, 5}},		
	{{false, false, DirectionGun::NONE}, {0, 0}},
	{{false, false, DirectionGun::LEFT}, {0, 0}},
	{{false, false, DirectionGun::RIGHT}, {0, 0}},
	{{false, false, DirectionGun::UP}, {0, 0}},
	{{false, false, DirectionGun::DOWN}, {0, 0}},
	{{false, false, DirectionGun::LEFT_UP}, {0, 0}},		
	{{false, false, DirectionGun::RIGHT_UP}, {0, 0}},		
	{{false, false, DirectionGun::LEFT_DOWN}, {0, 0}},	
	{{false, false, DirectionGun::RIGHT_DOWN}, {0, 0}},	
	{{false, true, DirectionGun::NONE}, {1, 5}},
	{{false, true, DirectionGun::LEFT}, {1, 5}},
	{{false, true, DirectionGun::RIGHT}, {1, 5}},
	{{false, true, DirectionGun::UP}, {1, 5}},
	{{false, true, DirectionGun::DOWN}, {1, 5}},
	{{false, true, DirectionGun::LEFT_UP}, {1, 5}},		
	{{false, true, DirectionGun::RIGHT_UP}, {1, 5}},		
	{{false, true, DirectionGun::LEFT_DOWN}, {1, 5}},
	{{false, true, DirectionGun::RIGHT_DOWN}, {1, 5}},	
};
RPGMob::RPGMob(std::shared_ptr<SpriteAnimationPlayer> sprite) : Enemy(sprite) {}
RPGMob::~RPGMob() {}
void RPGMob::Init() {
	m_MAX_HP = RPGMOB_MAX_HP;
	m_HP = m_MAX_HP;
	m_Speed = RPGMOB_SPEED_X;
	m_JumpForce = RPGMOB_JUMP_FORCE;
	m_Gravity = GRAVITY;
	m_sightRange = RPGMOB_SIGHT_RANGE;
	m_shootRange = RPGMOB_SHOOT_RANGE;
	m_ShootCooldownTime = RPGMOB_SHOOT_COOLDOWN_TIME;
	std::shared_ptr<TextureManager> texture = ResourceManagers::GetInstance()->GetTexture(RPGMOB_BULLET_SPRITE_PATH);
	m_BulletPool = std::make_shared<BulletPool>(RPGMOB_BULLET_POOL_SIZE, texture, SDL_FLIP_NONE);
}
void RPGMob::HandleInput(int keyMask) {
	keyMask |= KEY_DOWN;
	m_IsShooting = false;
	if (!IsAlive()) return;
	m_IsShooting = keyMask & KEY_SHOOT;
	m_CurrentDirectionGun = DirectionGun::FromKeyMask(keyMask);
	SetFlip(m_CurrentDirectionGun & DirectionGun::LEFT ? SDL_FLIP_HORIZONTAL : m_CurrentDirectionGun & DirectionGun::RIGHT ? SDL_FLIP_NONE : m_flip);
	m_Velocity.x = m_CurrentDirectionGun & DirectionGun::LEFT ? -m_Speed : m_CurrentDirectionGun & DirectionGun::RIGHT ? m_Speed : 0;
	if ((keyMask & KEY_JUMP) && m_IsOnGround) {
		m_Velocity.y = -m_JumpForce;
	}
}
void RPGMob::Shoot() {
	std::shared_ptr<Bullet> bullet = m_BulletPool->SpawnBullet(RPGMOB_BULLET_DAMAGE);
	bool isCrouch = m_IsOnGround && m_CurrentDirectionGun == DirectionGun::DOWN;
	if (bullet) {
		bullet->SetSize(RPGMOB_BULLET_WIDTH, RPGMOB_BULLET_HEIGHT);
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
		velocityBullet = velocityBullet.Normalize() * RPGMOB_BULLET_SPEED;
		Vector2 p;
		p.x = Get2DPosition().x + GetWidth() / 2.0f + (-bullet->GetWidth() / 2.0f);
		p.y = Get2DPosition().y + static_cast<float>(GetHeight()) / ORIGINAL_RPGMOB_SIZE_H * 26 + (-bullet->GetHeight() / 2.0f);
		p += velocityBullet / velocityBullet.Length() * 11 * GetWidth() / ORIGINAL_RPGMOB_SIZE_W;
		bullet->Set2DPosition(p.x, p.y);
		bullet->SetVelocity(velocityBullet);
		bullet->SetRotation(atan2(velocityBullet.y, velocityBullet.x) * (180.0 / M_PI));
	}
	m_animationPlayer->SetCurrentFrame(m_animationPlayer->GetStartFrame());
	Sound::GetInstance()->LoadSfx(RPG_Sound);
	Sound::GetInstance()->PlaySfx(RPG_Sound);
}
void RPGMob::Update(float deltatime) {
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
	position.y += m_Velocity.y * deltatime;
	Set2DPosition(position.x, position.y);
	m_animationPlayer->Update(deltatime);
}
void RPGMob::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
	AnimationKey key{ m_IsOnGround, m_IsShooting, m_CurrentDirectionGun };
	auto it = s_AnimationMap.find(key);
	if (!IsAlive()) {
		m_animationPlayer->SetFrame(6, 9);
		m_animationPlayer->SetFrameTime(150);
		m_animationPlayer->SetLoop(false);
	}
	else if (it != s_AnimationMap.end()) {
		m_animationPlayer->SetFrame(it->second.first, it->second.second);
		m_animationPlayer->SetLoop(false);
	}
	m_animationPlayer->Draw(renderer, clip);
	m_Displacement = { 0, 0 };
	SDL_FRect colliderRect = GetColliderFRect();
	if (IsAlive()) {
		DrawHPBar(renderer, { colliderRect.x, Get2DPosition().y + static_cast<float>(GetHeight()) * ORIGINAL_ARMOB_Y / ORIGINAL_ARMOB_SIZE_H - 6 }, m_HP, m_MAX_HP, colliderRect.w, 3);
	}
}
SDL_Rect RPGMob::GetColliderRect() {
	SDL_FRect rect = GetColliderFRect();
	return {
		static_cast<int>(rect.x),
		static_cast<int>(rect.y),
		static_cast<int>(rect.w),
		static_cast<int>(rect.h)
	};
}
SDL_FRect RPGMob::GetColliderFRect() {
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
		rect.x + ORIGINAL_RPGMOB_CROUCH_X * rect.w / ORIGINAL_RPGMOB_SIZE_W,
		rect.y + ORIGINAL_RPGMOB_CROUCH_Y * rect.h / ORIGINAL_RPGMOB_SIZE_H,
		rect.w * ORIGINAL_RPGMOB_CROUCH_W / ORIGINAL_RPGMOB_SIZE_W,
		rect.h * ORIGINAL_RPGMOB_CROUCH_H / ORIGINAL_RPGMOB_SIZE_H
	} : SDL_FRect{
		rect.x + ORIGINAL_RPGMOB_X * rect.w / ORIGINAL_RPGMOB_SIZE_W,
		rect.y + ORIGINAL_RPGMOB_Y * rect.h / ORIGINAL_RPGMOB_SIZE_H,
		rect.w * ORIGINAL_RPGMOB_W / ORIGINAL_RPGMOB_SIZE_W,
		rect.h * ORIGINAL_RPGMOB_H / ORIGINAL_RPGMOB_SIZE_H
	};
}