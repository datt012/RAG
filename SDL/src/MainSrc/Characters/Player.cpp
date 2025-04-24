#include "Player.h"
#include "KeyDirection.h"
#include <algorithm>
#include <MainSrc/Collision/Collision.h>

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

void Player::SetCurrentAction(Action action)
{
	if (m_CurrentAction != FALL)
	{
		m_CurrentAction = action;
	}
}

void Player::HandleInput(int keyMask)
{
	// Handle input for movement
	if ((keyMask & KEY_LEFT) && (keyMask & KEY_RIGHT) == false)
	{
		m_Facing = LEFT;
		m_CurrentDirectionGun = DIR_HORIZONTAL;
		this->SetCurrentAction(RUN);

		m_VelocityX = -m_Speed;
	}
	if ((keyMask & KEY_RIGHT) && (keyMask & KEY_LEFT) == false)
	{
		m_Facing = RIGHT;
		m_CurrentDirectionGun = DIR_HORIZONTAL;
		this->SetCurrentAction(RUN);

		m_VelocityX = m_Speed;
	}
	if ((keyMask & KEY_RIGHT) == false && (keyMask & KEY_LEFT) == false)
	{
		m_CurrentDirectionGun = DIR_HORIZONTAL;

		m_VelocityX = 0;
	}


	if ((keyMask & KEY_UP))
	{
		this->SetCurrentAction(IDLE);
		m_CurrentDirectionGun = DIR_UP;
	}
	if ((keyMask & KEY_UP) && ((keyMask & KEY_LEFT) ^ (keyMask & KEY_RIGHT)))
	{
		this->SetCurrentAction(RUN);
		m_CurrentDirectionGun = DIR_DIAGONAL_UP;
	}

	if ((keyMask & KEY_DOWN))
	{
		this->SetCurrentAction(CROUCH);
		m_CurrentDirectionGun = DIR_HORIZONTAL;
	}

	if ((keyMask & KEY_DOWN) && ((keyMask & KEY_LEFT) ^ (keyMask & KEY_RIGHT)))
	{
		this->SetCurrentAction(RUN);
		m_CurrentDirectionGun = DIR_DIAGONAL_DOWN;
	}

	if (keyMask & KEY_JUMP)
	{
		if (m_CurrentAction != FALL && !m_IsJumping)
		{
			m_CurrentAction = FALL;
			m_CurrentDirectionGun = DIR_HORIZONTAL;

			m_VelocityY = -m_JumpForce;
			m_IsJumping = true;
		}
	}

	if (keyMask & KEY_SHOOT)
	{
		m_IsShooting = true;
	}
	else
	{
		m_IsShooting = false;
	}

	if ((keyMask & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)) == false)
	{
		this->SetCurrentAction(IDLE);
		//m_CurrentDirectionGun = DIR_HORIZONTAL;

		//if (m_CurrentAction != FALL)
		//{
		//	m_VelocityX = 0;
		//	m_VelocityY = 0;
		//}
	}
}

void Player::Update(float deltatime)
{
	this->ApplyMovement(deltatime);
	this->UpdateAnimation(deltatime);

	m_animationPlayer->Update(deltatime);
	//m_animationPlayer->Set2DPosition(
	//	std::min(std::max(0.0f, m_animationPlayer->Get2DPosition().x), SCREEN_WIDTH * 1.0f),
	//	std::min(std::max(0.0f, m_animationPlayer->Get2DPosition().y), SCREEN_HEIDHT * 1.0f)
	//);
	m_position = m_animationPlayer->GetPosition();
}

void Player::ApplyMovement(float deltaTime)
{
	Vector2 position = m_animationPlayer->Get2DPosition();
	position.x += m_VelocityX * deltaTime;
	position.y += m_VelocityY * deltaTime;

	m_animationPlayer->Set2DPosition(position.x, position.y);

	m_VelocityY += m_Gravity * deltaTime;
	m_VelocityX = m_VelocityX * 0.98f;
	m_VelocityY = m_VelocityY * 0.98f;
}

void Player::UpdateAnimation(float deltaTime)
{
	// Facing direction
	if (m_Facing == LEFT)
	{
		this->SetFlip(SDL_FLIP_HORIZONTAL);
	}
	else
	{
		this->SetFlip(SDL_FLIP_NONE);
	}

	// IDLE
	if (m_CurrentAction == IDLE && m_CurrentDirectionGun == DIR_HORIZONTAL)
	{
		if (m_IsShooting)
		{
			m_animationPlayer->SetFrame(18, 19);
		}
		else
		{
			m_animationPlayer->SetFrame(0, 5);
		}
	}

	// GUN UP
	if (m_CurrentAction == IDLE && m_CurrentDirectionGun == DIR_UP)
	{
		if (m_IsShooting)
		{
			m_animationPlayer->SetFrame(20, 21);
		}
		else
		{
			m_animationPlayer->SetFrame(21, 21);
		}
	}

	// RUN
	if (m_CurrentAction == RUN && m_CurrentDirectionGun == DIR_HORIZONTAL)
	{
		if (m_IsShooting)
		{
			m_animationPlayer->SetFrame(24, 31);
		}
		else
		{
			m_animationPlayer->SetFrame(6, 13);
		}
	}

	// RUN with DIR_DIAGONAL_UP
	if (m_CurrentAction == RUN && m_CurrentDirectionGun == DIR_DIAGONAL_UP)
	{
		if (m_IsShooting)
		{
			m_animationPlayer->SetFrame(56, 63);
		}
		else
		{
			m_animationPlayer->SetFrame(6, 13);
		}
	}

	// CROUCH
	if (m_CurrentAction == CROUCH && m_CurrentDirectionGun == DIR_HORIZONTAL)
	{
		if (m_IsShooting)
		{
			m_animationPlayer->SetFrame(22, 23);
		}
		else
		{
			m_animationPlayer->SetFrame(14, 14);
		}
	}

	// RUN (CROUCH) with DIR_DIAGONAL_DOWN
	if (m_CurrentAction == RUN && m_CurrentDirectionGun == DIR_DIAGONAL_DOWN)
	{
		if (m_IsShooting)
		{
			m_animationPlayer->SetFrame(64, 71);
		}
		else
		{
			m_animationPlayer->SetFrame(6, 13);
		}
	}

	// FALL
	if (m_CurrentAction == FALL)
	{
		if (m_IsShooting)
		{
			m_animationPlayer->SetFrame(32, 33);
		}
		else
		{
			m_animationPlayer->SetFrame(34, 35);
		}
	}

	// JUMP
	if (m_CurrentAction == FALL && m_IsJumping)
	{
		if (m_IsShooting)
		{
			m_animationPlayer->SetFrame(40, 41);
		}
		else
		{
			m_animationPlayer->SetFrame(15, 15);
		}
	}
}

bool Player::CheckCollisionAndResolve(std::shared_ptr<Map> map)
{
	bool res = false;
	bool isOnGround = false;

	m_animationPlayer->Set2DPosition(
		std::min(std::max(0.0f, m_animationPlayer->Get2DPosition().x), map->GetWidth() - GetWidth() * 1.0f),
		std::min(std::max(0.0f, m_animationPlayer->Get2DPosition().y), map->GetHeight() - GetHeight() * 1.0f)
	);
	m_position = m_animationPlayer->GetPosition();

	SDL_Rect playerRect = {
		static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		GetWidth(),
		GetHeight()
	};

	if (!map) return false;

	auto layer = map->GetCollisionLayer();
	if (!layer) return false;

	for (Pixel& pixel : layer->GetPixels()) {
		SDL_Rect pixelRect = {
			static_cast<int>(pixel.GetPosition().x),
			static_cast<int>(pixel.GetPosition().y),
			PIXEL_WIDTH,
			PIXEL_HEIGHT
		};

		if (Collision::CheckAABB(playerRect, pixelRect)) {
			SDL_Point push = Collision::GetPushVector(playerRect, pixelRect);
			m_position.x += push.x;
			m_position.y += push.y;
			m_animationPlayer->Set2DPosition(m_position.x, m_position.y);
			playerRect = {
				static_cast<int>(m_position.x),
				static_cast<int>(m_position.y),
				GetWidth(),
				GetHeight()
			};

			if (Collision::IsOnGround(playerRect, pixelRect)) {
				isOnGround = true;
			}

			res = true;
		}
	}
	if (isOnGround) {
		m_VelocityY = 0;
		m_IsJumping = false;
		m_CurrentAction = IDLE;
	}
	else {
		m_IsJumping = true;
		//m_CurrentAction = FALL;
	}

	return res;
}

void Player::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{
	printf("Player velocity: %f, %f\n", m_VelocityX, m_VelocityY);
	m_animationPlayer->Draw(renderer, clip);
}