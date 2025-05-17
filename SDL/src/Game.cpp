#include <stdio.h>
#include <string>
#include "Game.h"
#include "GameStateBase.h"
//Screen dimension constants
#include"SDL_mixer.h"
#include"KeyState.h"
#include "ResourceManagers.h"
#include "Renderer.h"

const float targetTime = 1000.f / LIMIT_FPS;

Game::Game()
{
	//Init create window for rendering
	Init();
}
bool Game::Init()
{	

	bool success = Renderer::GetInstance()->Init();
	GameStateMachine::GetInstance()->PushState(StateType::STATE_INTRO);
	return success;
}

void Game::Run()
{
	//Main loop flag
	bool quit = false;
	//Event handler
	SDL_Event e;

	fpsTimer = std::make_shared<Timer>();
	capTimer = std::make_shared<Timer>();

	int countedFrames = 0;
	fpsTimer->Start();
	Uint32 frameTicks = 0;
	while (!quit)
	{
		//Start cap timer
		capTimer->Start();
		float begin = SDL_GetTicks();
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{

			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (GameStateMachine::GetInstance()->HasState())
			{
				GameStateMachine::GetInstance()->CurrentState()->HandleKeyEvents(e);
			}

			//Handle Touch Event
			if (GameStateMachine::GetInstance()->HasState())
			{
				GameStateMachine::GetInstance()->CurrentState()->HandleTouchEvents(e);
			}

		}

		KeyState::HandleKeyState();

		//Calculate and correct fps
		float avgFPS = countedFrames / (fpsTimer->GetTicks() / 1000.f);
		if (fpsTimer->GetTicks() > 1000)
		{
			fpsTimer->Start();
			countedFrames = 0;
		}
		printf("Average Frames Per Second (With Cap): %.2f\n", avgFPS);

		//Update screen
		Update(frameTicks);
		Render();
		++countedFrames;

		//If frame finished early
		frameTicks = capTimer->GetTicks();
		if (frameTicks < targetTime)
		{
			//Wait remaining time
			SDL_Delay(targetTime - frameTicks);
		}
	}
}

void Game::Update(float deltaTime)
{
	GameStateMachine::GetInstance()->PerformStateChange();
	if (GameStateMachine::GetInstance()->HasState())
	{
		GameStateMachine::GetInstance()->CurrentState()->Update(deltaTime);
	}
}

void Game::Render()
{
	if (GameStateMachine::GetInstance()->HasState())
	{
		//Clear screen
		SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(Renderer::GetInstance()->GetRenderer());  // function fills the screen with the color that was last set with SDL_SetRenderDrawColor
		GameStateMachine::GetInstance()->CurrentState()->Draw(Renderer::GetInstance()->GetRenderer());
		SDL_RenderPresent(Renderer::GetInstance()->GetRenderer());
	}
}

Game::~Game()
{
	Renderer::GetInstance()->FreeInstance();
	ResourceManagers::GetInstance()->FreeInstance();
	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

