#include "GameOver.h"
#include "ResourceManagers.h"
#include "GameStateMachine.h"
#include "Sound.h"
#include "MainSrc/Level/Level.h"
GameOver::GameOver() : GameStateBase(StateType::STATE_OVER) {}

GameOver::~GameOver() {
    Exit();
}

void GameOver::Init() {

    auto texture = ResourceManagers::GetInstance()->GetTexture("paper.png");
    m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
    m_background->Set2DPosition(0, 0);


    SDL_Color main = { 76, 185, 23 };
    SDL_Color white = { 255, 255, 255 };
    auto fontBig = ResourceManagers::GetInstance()->GetFont("f1.ttf", 60);
    auto fontMid = ResourceManagers::GetInstance()->GetFont("f1.ttf", 40);


    m_titleText = std::make_shared<Text>("Game Over", fontBig, main);
    m_titleText->SetSize(300, 70);
    m_titleText->Set2DPosition((SCREEN_WIDTH - 300) / 2, 100);


    texture = ResourceManagers::GetInstance()->GetTexture("ok.png");
    auto btnBack = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
    btnBack->SetSize(80, 80);
    btnBack->Set2DPosition((SCREEN_WIDTH - 100) / 2 + btnBack->GetWidth()/4, SCREEN_HEIDHT - 140);
    btnBack->SetOnClick([]() {
        Sound::GetInstance()->StopSound();
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
        Level::GetInstance()->SetLevel(1); 
    });
    m_listBtn.push_back(btnBack);
    Sound::GetInstance()->StopSound();
    Sound::GetInstance()->LoadSound("over.wav");
    Sound::GetInstance()->PlaySound("over.wav");
}

void GameOver::Exit() {
    m_background = nullptr;
    m_titleText = nullptr;
    m_backText = nullptr;
    m_listBtn.clear();
}

void GameOver::Pause() {}

void GameOver::Resume() {}

void GameOver::HandleEvents() {}

void GameOver::HandleKeyEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
        Sound::GetInstance()->StopSound();
        GameStateMachine::GetInstance()->PushState(StateType::STATE_MENU);
    }
}

void GameOver::HandleTouchEvents(SDL_Event& e) {
    for (auto& btn : m_listBtn) {
        if (btn->HandleTouchEvent(&e)) break;
    }
}

void GameOver::HandleMouseMoveEvents(int x, int y) {}

void GameOver::Update(float deltaTime) {
    m_background->Update(deltaTime);
    for (auto& btn : m_listBtn) btn->Update(deltaTime);
}

void GameOver::Draw(SDL_Renderer* renderer) {
    m_background->Draw(renderer);

    for (auto& btn : m_listBtn) btn->Draw(renderer);

    if (m_titleText) m_titleText->Draw(renderer);
    if (m_backText) m_backText->Draw(renderer);
}