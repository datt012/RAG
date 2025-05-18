#include <stdio.h>
#include <string>
#include "Game.h"
#include "GameStateBase.h"
#include"SDL_mixer.h"
#include"KeyState.h"
#include "ResourceManagers.h"
#include "Renderer.h"
const float targetTime = 1000.f / LIMIT_FPS;
Game::Game(){
	Init();
}
bool Game::Init() {
	bool success = Renderer::GetInstance()->Init();
	GameStateMachine::GetInstance()->PushState(StateType::STATE_INTRO);
	return success;
}
void Game::Run() {
	bool quit = false;
	SDL_Event e;
	fpsTimer = std::make_shared<Timer>();
	capTimer = std::make_shared<Timer>();
	int countedFrames = 0;
	fpsTimer->Start();
	Uint32 frameTicks = 0;
	while (!quit) {
		capTimer->Start();
		float begin = SDL_GetTicks();
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (GameStateMachine::GetInstance()->HasState()) {
				GameStateMachine::GetInstance()->CurrentState()->HandleKeyEvents(e);
			}
			if (GameStateMachine::GetInstance()->HasState()) {
				GameStateMachine::GetInstance()->CurrentState()->HandleTouchEvents(e);
			}
		}
		KeyState::HandleKeyState();
		Update(frameTicks);
		Render();
		++countedFrames;
		frameTicks = capTimer->GetTicks();
		if (frameTicks < targetTime) {
			SDL_Delay(targetTime - frameTicks);
		}
	}
}
void Game::Update(float deltaTime) {
	GameStateMachine::GetInstance()->PerformStateChange();
	if (GameStateMachine::GetInstance()->HasState()) {
		GameStateMachine::GetInstance()->CurrentState()->Update(deltaTime);
	}
}
void Game::Render() {
	if (GameStateMachine::GetInstance()->HasState()) {
		SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(Renderer::GetInstance()->GetRenderer());
		GameStateMachine::GetInstance()->CurrentState()->Draw(Renderer::GetInstance()->GetRenderer());
		SDL_RenderPresent(Renderer::GetInstance()->GetRenderer());
	}
}
Game::~Game() {
	Renderer::GetInstance()->FreeInstance();
	ResourceManagers::GetInstance()->FreeInstance();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

