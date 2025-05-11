#include "Complete.h"
#include "ResourceManagers.h"
#include "GameStateMachine.h"
#include "Sound.h"
#include "MainSrc/LevelDesign/LevelStates.h"

Complete::Complete() : GameStateBase(StateType::STATE_COMPLETE) {}

Complete::~Complete() {
    Exit();
}

void Complete::Init() {
    // Load background
    auto texture = ResourceManagers::GetInstance()->GetTexture("paper.png");
    m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
    m_background->Set2DPosition(0, 0);

    // Load font and color
    SDL_Color red = { 255, 0, 0 };
    SDL_Color white = { 255, 255, 255 };
    auto fontBig = ResourceManagers::GetInstance()->GetFont("f1.ttf", 60);
    auto fontMid = ResourceManagers::GetInstance()->GetFont("f1.ttf", 40);

    // Title text: "Game Over"
    m_titleText = std::make_shared<Text>("Game Complete!", fontBig, red);
    m_titleText->SetSize(300, 70);
    m_titleText->Set2DPosition((SCREEN_WIDTH - 300) / 2, 100);

    // Back button
    texture = ResourceManagers::GetInstance()->GetTexture("button_red.png");
    auto btnBack = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
    btnBack->SetSize(100, 60);
    btnBack->Set2DPosition((SCREEN_WIDTH - 100) / 2, SCREEN_HEIDHT - 140);
    btnBack->SetOnClick([]() {
        printf("[GameOver] Back to menu clicked.\n");
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
        LevelStates::GetInstance()->SetLevelState(1);
        });
    m_listBtn.push_back(btnBack);

    // Back text: "OK"
    m_backText = std::make_shared<Text>("OK", fontMid, white);
    m_backText->SetSize(50, 50);

    float btnX = btnBack->Get2DPosition().x;
    float btnY = btnBack->Get2DPosition().y;
    float btnW = btnBack->GetWidth();
    float btnH = btnBack->GetHeight();
    float textW = m_backText->GetWidth();
    float textH = m_backText->GetHeight();

    m_backText->Set2DPosition(
        btnX + (btnW - textW) / 2,
        btnY + (btnH - textH) / 2
    );

    
    Sound::GetInstance()->StopSound();
    Sound::GetInstance()->LoadSound("victory.mp3");
    Sound::GetInstance()->PlaySound("victory.mp3");
}

void Complete::Exit() {
    m_background = nullptr;
    m_titleText = nullptr;
    m_backText = nullptr;
    m_listBtn.clear();
    Sound::GetInstance()->StopSound();
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
    if (m_background) m_background->Draw(renderer);
    if (m_titleText) m_titleText->Draw(renderer);
    

    for (auto& btn : m_listBtn) {
        btn->Draw(renderer);
    }
    if (m_backText) m_backText->Draw(renderer);
}
