#include <stdio.h>
#include <string>
#include "Game.h"
#include "GameStateBase.h"
//Screen dimension constants
#include"SDL_mixer.h"
#include"KeyState.h"
#include "ResourceManagers.h"
#include "Renderer.h"
const Uint32 targetTime = 1 / LIMIT_FPS;
Game::Game()
{
	//Init create window for rendering
	Init();
}
bool Game::Init()
{
	bool success=	Renderer::GetInstance()->Init();
	GameStateMachine::GetInstance()->PushState(StateType::STATE_INTRO);
	return success;
}

void Game::Run()
{
			//Main loop flag
			bool quit = false;
			//Event handler
			SDL_Event e;
			Uint32 lastUpdate = SDL_GetTicks();
			while (!quit)
			{
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
				
				Uint32 current = SDL_GetTicks();

				float dT = (current - lastUpdate) / 1000.0f; // to convert to seconds

				lastUpdate = current;
				////Limit FPS
				if (dT < targetTime)
				{
					Update(targetTime);
					SDL_Delay(targetTime - dT);
				}
				else
				{
					Update(dT);	
				}
				//Update screen
				SDL_RenderPresent(Renderer::GetInstance()->GetRenderer());
			}
}

void Game::Update(float deltaTime)
{
	GameStateMachine::GetInstance()->PerformStateChange();
	if (GameStateMachine::GetInstance()->HasState())
	{
		GameStateMachine::GetInstance()->CurrentState()->Update(deltaTime);
		//Clear screen
		SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(Renderer::GetInstance()->GetRenderer());  // function fills the screen with the color that was last set with SDL_SetRenderDrawColor
		GameStateMachine::GetInstance()->CurrentState()->Draw(Renderer::GetInstance()->GetRenderer());
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

