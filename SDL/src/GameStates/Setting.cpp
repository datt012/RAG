#include "Setting.h"
#include "ResourceManagers.h"
#include "GameStateMachine.h"
#include "Sound.h"
#include <iostream>
using namespace std;

Setting::Setting() : GameStateBase(StateType::STATE_OPTION) {}

Setting::~Setting() {
    Exit();
}

void Setting::Init() {
    auto texture = ResourceManagers::GetInstance()->GetTexture("paper.png");
    m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
    m_background->Set2DPosition(0, 0);


    SDL_Color white = { 255, 255, 255 };
    auto fontBig = ResourceManagers::GetInstance()->GetFont("f1.ttf", 50);


    m_titleText = std::make_shared<Text>("Settings", fontBig, white);
    m_titleText->SetSize(300, 60);
    m_titleText->Set2DPosition((SCREEN_WIDTH - 300) / 2, 80);


    m_soundText = std::make_shared<Text>("Music", fontBig, white);
    m_soundText->SetSize(150, 50);
    m_soundText->Set2DPosition(330, 200);


    m_SFXText = std::make_shared<Text>("SFX", fontBig, white);
    m_SFXText->SetSize(150, 50);
    m_SFXText->Set2DPosition(330, 300);


    texture = ResourceManagers::GetInstance()->GetTexture(Sound::GetInstance()->GetFlag() ? "on.png" : "off.png");
    m_soundIMG = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_soundIMG->SetSize(100, 50);
    m_soundIMG->Set2DPosition(530, 200);
    std::cout << "init: " << Sound::GetInstance()->GetFlag() << std::endl;


    texture = ResourceManagers::GetInstance()->GetTexture(Sound::GetInstance()->GetSFXFlag() ? "on.png" : "off.png");
    m_SFXIMG = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_SFXIMG->SetSize(100, 50);
    m_SFXIMG->Set2DPosition(530, 300);


    texture = ResourceManagers::GetInstance()->GetTexture("ok.png");
    auto btnBack = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
    btnBack->SetSize(80, 80);
    btnBack->Set2DPosition((SCREEN_WIDTH - 100) / 2 + btnBack->GetWidth()/4, SCREEN_HEIDHT - 120);
    btnBack->SetOnClick([]() {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
    });
    m_listBtn.push_back(btnBack);
    Sound::GetInstance()->StopSound();
    Sound::GetInstance()->LoadSound("Alarm01.mp3");
    Sound::GetInstance()->PlaySound("Alarm01.mp3");
}

void Setting::Exit() {
    m_background = nullptr;
    m_titleText = nullptr;
    m_backText = nullptr;
    m_soundText = nullptr;
    m_SFXText = nullptr;
    m_soundIMG = nullptr;
    m_SFXIMG = nullptr;
    m_listBtn.clear();
}

void Setting::Pause() {
    Sound::GetInstance()->PauseSound();
}

void Setting::Resume() {
    if (Sound::GetInstance()->GetFlag()) Sound::GetInstance()->ResumeSound();
}

void Setting::HandleEvents() {}

void Setting::HandleKeyEvents(SDL_Event& e) {
    /*if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
    }*/
}

void Setting::HandleTouchEvents(SDL_Event& e) {
    for (auto& btn : m_listBtn) {
        if (btn->HandleTouchEvent(&e)) break;
    }

    if (e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);


        SDL_Rect soundRect = {
            (int)m_soundIMG->Get2DPosition().x,
            (int)m_soundIMG->Get2DPosition().y,
            (int)m_soundIMG->GetWidth(),
            (int)m_soundIMG->GetHeight()
        };
        if (x >= soundRect.x && x <= soundRect.x + soundRect.w &&
            y >= soundRect.y && y <= soundRect.y + soundRect.h) {

            bool flag = Sound::GetInstance()->GetFlag();
            Sound::GetInstance()->SetFlag(!flag);

            // Lấy flag mới sau khi cập nhật
            bool newFlag = Sound::GetInstance()->GetFlag();

            auto tex = ResourceManagers::GetInstance()->GetTexture(newFlag ? "on.png" : "off.png");
            m_soundIMG->SetTexture(tex);

            newFlag ? Sound::GetInstance()->ResumeSound() : Sound::GetInstance()->PauseSound();

            std::cout << "New Flag: " << newFlag << std::endl;
        }



        SDL_Rect sfxRect = {
            (int)m_SFXIMG->Get2DPosition().x,
            (int)m_SFXIMG->Get2DPosition().y,
            (int)m_SFXIMG->GetWidth(),
            (int)m_SFXIMG->GetHeight()
        };
        if (x >= sfxRect.x && x <= sfxRect.x + sfxRect.w &&
            y >= sfxRect.y && y <= sfxRect.y + sfxRect.h) {

            bool flag = Sound::GetInstance()->GetSFXFlag();
            Sound::GetInstance()->SetSFXFlag(!flag);

            // Lấy flag mới sau khi cập nhật
            bool newFlag = Sound::GetInstance()->GetSFXFlag();

            auto tex = ResourceManagers::GetInstance()->GetTexture(newFlag ? "on.png" : "off.png");
            m_SFXIMG->SetTexture(tex);

            

            std::cout << "New Flag: " << newFlag << std::endl;
            

        }
    }
}

void Setting::HandleMouseMoveEvents(int x, int y) {}

void Setting::Update(float deltaTime) {
    m_background->Update(deltaTime);
    for (auto& btn : m_listBtn) btn->Update(deltaTime);
}

void Setting::Draw(SDL_Renderer* renderer) {
    m_background->Draw(renderer);

    for (auto& btn : m_listBtn) btn->Draw(renderer);

    if (m_titleText) m_titleText->Draw(renderer);
    if (m_backText) m_backText->Draw(renderer);
    if (m_soundText) m_soundText->Draw(renderer);
    if (m_SFXText) m_SFXText->Draw(renderer);
    if (m_soundIMG) m_soundIMG->Draw(renderer);
    if (m_SFXIMG) m_SFXIMG->Draw(renderer);
}