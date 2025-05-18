#include "Complete.h"
#include "ResourceManagers.h"
#include "GameStateMachine.h"
#include "Sound.h"
#include "MainSrc/Level/Level.h"
Complete::Complete() : GameStateBase(StateType::STATE_COMPLETE) {}
Complete::~Complete() {
    Exit();
}
void Complete::Init() {
    auto texture = ResourceManagers::GetInstance()->GetTexture("complete.png");
    m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
    m_background->Set2DPosition(0, 0);
    SDL_Color main = { 76, 185, 23 };
    SDL_Color white = { 255, 255, 255 };
    auto fontBig = ResourceManagers::GetInstance()->GetFont("font.ttf", 80);
    auto fontMid = ResourceManagers::GetInstance()->GetFont("font.ttf", 40);
    m_titleText = std::make_shared<Text>("Game Complete", fontBig, main);
    m_titleText->SetSize(500, 120);
    m_titleText->Set2DPosition((SCREEN_WIDTH - m_titleText->GetWidth()) / 2, 100);
    texture = ResourceManagers::GetInstance()->GetTexture("btn_ok.png");
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
    Sound::GetInstance()->LoadSound("victory.wav");
    Sound::GetInstance()->PlaySound("victory.wav");
}
void Complete::Exit() {
    m_background = nullptr;
    m_titleText = nullptr;
    m_backText = nullptr;
    m_listBtn.clear();
}
void Complete::Pause() {
    Sound::GetInstance()->PauseSound();
}
void Complete::Resume() {
    Sound::GetInstance()->ResumeSound();
}
void Complete::HandleEvents() {}
void Complete::HandleKeyEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
    }
}
void Complete::HandleTouchEvents(SDL_Event& e) {
    for (auto& btn : m_listBtn) {
        if (btn->HandleTouchEvent(&e)) break;
    }
}
void Complete::HandleMouseMoveEvents(int x, int y) {}
void Complete::Update(float deltaTime) {
    m_background->Update(deltaTime);
    for (auto& btn : m_listBtn) {
        btn->Update(deltaTime);
    }
}
void Complete::Draw(SDL_Renderer* renderer) {
    if (m_background) {
        m_background->Draw(renderer);
    }
    if (m_titleText) {
        m_titleText->Draw(renderer);
    }
    for (auto& btn : m_listBtn) {
        btn->Draw(renderer);
    }
    if (m_backText) {
        m_backText->Draw(renderer);
    }
}