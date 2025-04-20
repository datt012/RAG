#include "GSPlay.h"

GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{
}


void GSPlay::Init()
{	
	//// map init
	mapp = std::make_shared<Map>();
	mapp->init("Data\\Asset\\test\\main.tmx");

	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play1.tga");

	// background
	m_background = std::make_shared<Sprite2D>( texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);

	// button close
	texture = ResourceManagers::GetInstance()->GetTexture("btn_close.tga");
	button = std::make_shared<MouseButton>( texture, SDL_FLIP_NONE);
	button->SetSize(50, 50);
	button->Set2DPosition(SCREEN_WIDTH - 50, 10);
	button->SetOnClick([this]() {
		GameStateMachine::GetInstance()->PopState();
		});
	m_listButton.push_back(button);


	texture = ResourceManagers::GetInstance()->GetTexture("SpriteSheet_player_7_8.tga");
	animation = std::make_shared<SpriteAnimationPlayer>(texture, 7, 8, 0, 0, 30);
	player = std::make_shared<Player>(animation);
	player->SetSize(70, 70);
	player->Set2DPosition(300, 300);
	m_listPlayer.push_back(player);

	Camera::GetInstance()->SetLevelDimension(SCREEN_WIDTH * 5, SCREEN_HEIDHT * 5);
	Camera::GetInstance()->SetTarget(player);
	
	m_KeyPress = 0;
}

void GSPlay::Exit()
{
}


void GSPlay::Pause()
{

}
void GSPlay::Resume()
{
}


void GSPlay::HandleEvents()
{
}

void GSPlay::HandleKeyEvents(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN)//&& e.key.repeat == 0) //For e.key.repeat it's because key repeat is enabled by default and if you press and hold a key it will report multiple key presses. That means we have to check if the key press is the first one because we only care when the key was first pressed.
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)                                                                                            
		{
		case SDLK_a:
		case SDLK_LEFT:
			m_KeyPress |= KEY_LEFT;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			m_KeyPress |= KEY_DOWN;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			m_KeyPress |= KEY_RIGHT;
			break;
		case SDLK_w:
		case SDLK_UP:
			m_KeyPress |= KEY_UP;
			break;
		case SDLK_j:
			m_KeyPress |= KEY_SHOOT;
			break;
		case SDLK_k:
			m_KeyPress |= KEY_JUMP;
			break;
		default:
			break;
		}
	}
	////Key Up
	else if (e.type == SDL_KEYUP)//&& e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
		case SDLK_LEFT:
			m_KeyPress ^= KEY_LEFT;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			m_KeyPress ^= KEY_DOWN;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			m_KeyPress ^= KEY_RIGHT;
			break;
		case SDLK_w:
		case SDLK_UP:
			m_KeyPress ^= KEY_UP;
			break;
		case SDLK_j:
			m_KeyPress ^= KEY_SHOOT;
			break;
		case SDLK_k:
			m_KeyPress ^= KEY_JUMP;
			break;
		default:
			break;
		}
	}
}

void GSPlay::HandleTouchEvents(SDL_Event& e)
{
	for (auto button : m_listButton)
	{
		if (button->HandleTouchEvent(&e))
		{
			break;
		}
	}
}

void GSPlay::HandleMouseMoveEvents(int x, int y)
{
}

void GSPlay::Update(float deltaTime)
{
	switch (m_KeyPress)//Handle Key event start
	{
	default:
		break;
	}
	// Key State event end


	for (auto it : m_listPlayer)
	{
		it->HandleInput(m_KeyPress);
	}
	printf("Key Press: %d\n", m_KeyPress);

	// Update
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	for (auto it : m_listAnimation)
	{
		it->Update(deltaTime);
	}
	for (auto it : m_listPlayer)
	{
		it->Update(deltaTime);
	}

	Camera::GetInstance()->Update(deltaTime);
}

void GSPlay::Draw(SDL_Renderer* renderer)
{
	// m_background->Draw(renderer);
	mapp->render(renderer);

	for (auto it : m_listButton)
	{
		it->Draw(renderer);
	}
	for (auto it : m_listAnimation)
	{
		it->Draw(renderer);
	}
	for (auto it : m_listPlayer)
	{
		it->Draw(renderer);
	}
}