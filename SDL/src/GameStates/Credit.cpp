#include "Credit.h"
#include "GameStateMachine.h"
#include "ResourceManagers.h"
#include "Sound.h"
Credit::Credit() : GameStateBase(StateType::STATE_CREDIT) {}
Credit::~Credit() {
    Exit();
}
void Credit::Init() {
    auto texture = ResourceManagers::GetInstance()->GetTexture("background.png");
    m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
    m_background->Set2DPosition(0, 0);
    SDL_Color white = { 255, 255, 255 };
    auto fontBig = ResourceManagers::GetInstance()->GetFont("font.ttf", 50);
    auto fontMid = ResourceManagers::GetInstance()->GetFont("font.ttf", 40);
    m_text = std::make_shared<Text>("Create By: ", fontBig, white);
    m_text->SetSize(300, 60);
    m_text->Set2DPosition((SCREEN_WIDTH - m_text->GetWidth()) / 2 + 20, SCREEN_HEIDHT / 2 - 300);
    m_name1 = std::make_shared<Text>("Tran Tien Dat", fontMid, white);
    m_name1->SetSize(200, 40);
    m_name1->Set2DPosition((SCREEN_WIDTH - m_name1->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 190);
    m_name2 = std::make_shared<Text>("Nguyen Van Duy", fontMid, white);
    m_name2->SetSize(200, 40);
    m_name2->Set2DPosition((SCREEN_WIDTH - m_name2->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 150);
    m_newText1 = std::make_shared<Text>("-----------------", fontBig, white);
    m_newText1->SetSize(400, 30);
    m_newText1->Set2DPosition((SCREEN_WIDTH - m_newText1->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 100);
    m_newText2 = std::make_shared<Text>("How to play ?", fontBig, white);
    m_newText2->SetSize(200, 40);
    m_newText2->Set2DPosition((SCREEN_WIDTH - m_newText2->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 80);
    m_TextForMove = std::make_shared<Text>("Move: WASD or Arrow key", fontBig, white);
    m_TextForMove->SetSize(250, 40);
    m_TextForMove->Set2DPosition((SCREEN_WIDTH - m_TextForMove->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 40);
    m_TextForJump = std::make_shared<Text>("Jump: K", fontBig, white);
    m_TextForJump->SetSize(80, 40);
    m_TextForJump->Set2DPosition((SCREEN_WIDTH - m_TextForJump->GetWidth()) / 2, SCREEN_HEIDHT / 2 );
    m_TextForShoot = std::make_shared<Text>("Shoot: J", fontBig, white);
    m_TextForShoot->SetSize(100, 40);
    m_TextForShoot->Set2DPosition((SCREEN_WIDTH - m_TextForShoot->GetWidth()) / 2, SCREEN_HEIDHT / 2 + 40);
    m_ReferenceText = std::make_shared<Text>("Special thanks to everyone!", fontMid, white);
    m_ReferenceText->SetSize(400, 40);
    m_ReferenceText->Set2DPosition((SCREEN_WIDTH - m_ReferenceText->GetWidth()) / 2, SCREEN_HEIDHT / 2 + 140);
    texture = ResourceManagers::GetInstance()->GetTexture("btn_ok.png");
    auto btnBack = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
    btnBack->SetSize(80, 80);
    btnBack->Set2DPosition((SCREEN_WIDTH - 100) / 2 + btnBack->GetWidth()/4, SCREEN_HEIDHT - 120);
    btnBack->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
    });
    m_listBtn.push_back(btnBack);
    Sound::GetInstance()->LoadSound("intro.wav");
    Sound::GetInstance()->PlaySound("intro.wav");
}
void Credit::Exit() {
    m_background = nullptr;
    m_text = nullptr;
    m_name1 = nullptr;
    m_name2 = nullptr;
    m_ReferenceText = nullptr;
    m_listBtn.clear();
}
void Credit::Pause() {}
void Credit::Resume() {}
void Credit::HandleEvents() {}
void Credit::HandleKeyEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
    }
}
void Credit::HandleTouchEvents(SDL_Event& e) {
    for (auto& btn : m_listBtn) {
        if (btn->HandleTouchEvent(&e)) break;
    }
}
void Credit::HandleMouseMoveEvents(int x, int y) {}
void Credit::Update(float deltaTime) {
    m_background->Update(deltaTime);
    for (auto& btn : m_listBtn) {
        btn->Update(deltaTime);
    }
}
void Credit::Draw(SDL_Renderer* renderer) {
    m_background->Draw(renderer);
    for (auto& btn : m_listBtn) {
        btn->Draw(renderer);
    }
    if (m_text) {
        m_text->Draw(renderer);
    }
    if (m_name1) {
        m_name1->Draw(renderer);
    }
    if (m_name2) {
        m_name2->Draw(renderer);
    }
    if (m_ReferenceText) {
        m_ReferenceText->Draw(renderer);
    }
    if (m_newText1) {
        m_newText1->Draw(renderer);
    }
    if (m_newText2) {
        m_newText2->Draw(renderer);
    }
    if (m_TextForMove) {
        m_TextForMove->Draw(renderer);
    }
    if (m_TextForJump) {
        m_TextForJump->Draw(renderer);
    }
    if (m_TextForShoot) {
        m_TextForShoot->Draw(renderer);
    }
}