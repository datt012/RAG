#include "Credit.h"
#include "GameStateMachine.h"
#include "ResourceManagers.h"
#include "Sound.h"

Credit::Credit() : GameStateBase(StateType::STATE_CREDIT) {}

Credit::~Credit() {
    Exit();
}

void Credit::Init() {



    auto texture = ResourceManagers::GetInstance()->GetTexture("paper.png");


    m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
    m_background->Set2DPosition(0, 0);


    SDL_Color white = { 255, 255, 255 };
    auto fontBig = ResourceManagers::GetInstance()->GetFont("f1.ttf", 50);
    auto fontMid = ResourceManagers::GetInstance()->GetFont("f1.ttf", 40);



    // Thank-you text
    m_text = std::make_shared<Text>("Create By: ", fontBig, white);
    m_text->SetSize(300, 60);
    m_text->Set2DPosition((SCREEN_WIDTH - m_text->GetWidth()) / 2 + 20, SCREEN_HEIDHT / 2 - 250);


    // Author name 1
    m_name1 = std::make_shared<Text>("Tran Tien Dat", fontMid, white);
    m_name1->SetSize(200, 40);
    m_name1->Set2DPosition((SCREEN_WIDTH - m_name1->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 140);


    // Author name 2
    m_name2 = std::make_shared<Text>("Nguyen Van Duy", fontMid, white);
    m_name2->SetSize(200, 40);
    m_name2->Set2DPosition((SCREEN_WIDTH - m_name2->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 100);



    m_ReferenceText = std::make_shared<Text>("Special thanks to everyone!", fontMid, white);
    m_ReferenceText->SetSize(400, 40);
    m_ReferenceText->Set2DPosition((SCREEN_WIDTH - m_ReferenceText->GetWidth()) / 2, SCREEN_HEIDHT / 2 + 80);



    texture = ResourceManagers::GetInstance()->GetTexture("ok.png");



    auto btnBack = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
    btnBack->SetSize(80, 80);
    btnBack->Set2DPosition((SCREEN_WIDTH - 100) / 2 + btnBack->GetWidth()/4, SCREEN_HEIDHT - 120);
    btnBack->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
        });
    m_listBtn.push_back(btnBack);



    


    Sound::GetInstance()->LoadSound("Alarm01.mp3");
    Sound::GetInstance()->PlaySound("Alarm01.mp3");
}




void Credit::Exit() {
    m_background = nullptr;
    m_text = nullptr;
    m_name1 = nullptr;
    m_name2 = nullptr;
    m_ReferenceText = nullptr;
    m_listBtn.clear();
    
}

void Credit::Pause() {
  
}

void Credit::Resume() {
   
}

void Credit::HandleEvents() {}

void Credit::HandleKeyEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
    }
}

void Credit::HandleTouchEvents(SDL_Event& e) {
    for (auto& btn : m_listBtn) {
        if (btn->HandleTouchEvent(&e)) {
            break;
        }
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

    if (m_text) m_text->Draw(renderer);
    if (m_name1) m_name1->Draw(renderer);
    if (m_name2) m_name2->Draw(renderer);
    if (m_ReferenceText) m_ReferenceText->Draw(renderer);
    if (m_homeText) m_homeText->Draw(renderer);
}