#include "GSMenu.h"
#include "ResourceManagers.h"
#include "MouseButton.h"
#include "Setting.h"
#include "Camera.h"
GSMenu::GSMenu() : GameStateBase(StateType::STATE_MENU),
m_background(nullptr), m_listButton(std::list<std::shared_ptr<MouseButton>>{}), m_textGameName(nullptr) {}
GSMenu::~GSMenu() {}
bool GSMenu::m_isPause = false;
void GSMenu::Init() {
	auto texture = ResourceManagers::GetInstance()->GetTexture("menu.png");
	m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);
	texture = ResourceManagers::GetInstance()->GetTexture("btn_play.png");
	std::shared_ptr<MouseButton> btnPlay = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnPlay->SetSize(150, 150);
	btnPlay->Set2DPosition((SCREEN_WIDTH - btnPlay->GetWidth()) / 2, (SCREEN_HEIDHT - btnPlay->GetHeight()) / 2 + 200);
	texture = ResourceManagers::GetInstance()->GetTexture("btn_close.png");
	std::shared_ptr<MouseButton> btnClose = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnClose->SetSize(50, 50);
	btnClose->Set2DPosition(SCREEN_WIDTH - btnClose->GetWidth(), 10);
	btnClose->SetOnClick([]() {
		exit(0);
	});
	texture = ResourceManagers::GetInstance()->GetTexture("btn_settings.png");
	std::shared_ptr<MouseButton> btnOption = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnOption->SetSize(100, 100);
	btnOption->Set2DPosition((SCREEN_WIDTH - btnOption->GetWidth()) / 2 - 200, (SCREEN_HEIDHT / 3) * 2 + 50);
	btnOption->SetOnClick([]() {
		GameStateMachine::GetInstance()->PushState(StateType::STATE_OPTION);
	});
	texture = ResourceManagers::GetInstance()->GetTexture("btn_about.png");
	std::shared_ptr<MouseButton> btnCredit = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnCredit->Set2DPosition((SCREEN_WIDTH - btnCredit->GetWidth()) / 2 + 200, (SCREEN_HEIDHT / 3) * 2 + 50);
	btnCredit->SetSize(100, 100);
	btnCredit->SetOnClick([]() {
		GameStateMachine::GetInstance()->PushState(StateType::STATE_CREDIT);
	});
	m_textColor = { 76,185,23 };
	auto font = ResourceManagers::GetInstance()->GetFont("font.ttf", 20);
	m_textGameName = std::make_shared<Text>("Run and gun", font, m_textColor);
	m_textGameName->SetSize(300, 150);
	m_textGameName->Set2DPosition((SCREEN_WIDTH - m_textGameName->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 300);
	Sound::GetInstance()->LoadSound("intro.wav");
	Sound::GetInstance()->PlaySound("intro.wav");
	if (m_isPause == true) {
		btnPlay->SetOnClick([]() {
			GameStateMachine::GetInstance()->PopState(); 
			Sound::GetInstance()->StopSound();
		});
		SetPauseFlag(false);
		m_listButton.push_back(btnPlay);
		m_listButton.push_back(btnClose);
	}
	else {
		btnPlay->SetOnClick([]() {
			GameStateMachine::GetInstance()->PushState(StateType::STATE_PLAY);
		});
		SetPauseFlag(false);
		m_listButton.push_back(btnPlay);
		m_listButton.push_back(btnClose);
		m_listButton.push_back(btnOption);
		m_listButton.push_back(btnCredit);
	}
}
void GSMenu::Exit() {}
void GSMenu::Pause() {
	Sound::GetInstance()->PauseSound();
}
void GSMenu::Resume() {}
void GSMenu::HandleEvents() {}
void GSMenu::HandleKeyEvents(SDL_Event& e) {}
void GSMenu::HandleTouchEvents(SDL_Event& e) {
	for (auto button : m_listButton) {
		if (button->HandleTouchEvent(&e)) break;
	}
}
void GSMenu::HandleMouseMoveEvents(int x, int y) {}
void GSMenu::Update(float deltaTime) {
	m_background->Update(deltaTime);
	for (auto it : m_listButton) {
		it->Update(deltaTime);
	}
}
void GSMenu::Draw(SDL_Renderer* renderer) {
	m_background->Draw(renderer);
	for (auto it : m_listButton) {
		it->Draw(renderer);
	}
	m_textGameName->Draw(renderer);
}