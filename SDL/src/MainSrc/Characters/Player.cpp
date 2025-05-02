#include "Player.h"
#include "KeyDirection.h"
#include <algorithm>
#include <MainSrc/Collision/Collision.h>

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
	//m_animationPlayer->Init();
	//m_animationPlayer->SetFrame(0, 0); // Set initial frame to idle frame
	//m_animationPlayer->Set2DPosition(100, 100); // Set initial position
	//m_animationPlayer->SetSize(50, 50); // Set size
	//m_animationPlayer->SetRotation(0.0f); // Set rotation
	//m_animationPlayer->SetFlip(SDL_FLIP_NONE); // Set flip
}

void Player::HandleInput(int keyMask)
{
	m_IsShooting = keyMask & KEY_SHOOT;
	m_CurrentDirectionGun = DirectionGun::FromKeyMask(keyMask);
	printf("Direction Gun: %s\n", m_CurrentDirectionGun.to_string());

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

void Player::Update(float deltatime)
{
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
	if (it != s_AnimationMap.end())
	{
		m_animationPlayer->SetFrame(it->second.first, it->second.second);
	}

	m_animationPlayer->Draw(renderer, clip);

	m_Displacement = { 0, 0 };

	// Set color (eg: red)
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	SDL_Rect colliderRect = GetColliderRect();
	colliderRect.x -= Camera::GetInstance()->GetPosition().x;
	colliderRect.y -= Camera::GetInstance()->GetPosition().y;

	// Draw the bounding box
	SDL_RenderDrawRect(renderer, &colliderRect);

	// Reset color to default (eg: black)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}