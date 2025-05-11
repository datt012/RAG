#include "GSPlay.h"

GSPlay::GSPlay() : m_KeyPress(0) {}

GSPlay::~GSPlay() {}

void GSPlay::Init() {
    //// Lấy texture Enemy
    //auto textureEnemy = ResourceManagers::GetInstance()->GetTexture("ARMob.png");

    //// Khởi tạo Enemy
    //m_enemy = std::make_shared<Enemy>();
    //m_enemy->SetPosition(Vector2(300, 495));
    //m_enemy->SetFPositon(Vector2(300, 495));
    //m_enemy->Init2(textureEnemy, 100, 0.2f);

    

    //// enemy2
    //auto textureEnemy2 = ResourceManagers::GetInstance()->GetTexture("sprite.png");
    //m_enemy2 = std::make_shared<Enemy2>();
    //m_enemy2->SetPosition(Vector2(300, 150));
    //m_enemy2->SetFposition(Vector2(300, 150));
    //m_enemy2->Init2(textureEnemy2, 100, 0.2f);


    ////
    //auto textureEnemy3 = ResourceManagers::GetInstance()->GetTexture("enemy3.png");
    //m_enemy3 = std::make_shared<Enemy3>();
    //m_enemy3->SetPosition(Vector2(400, 150));
    //m_enemy3->Init2(textureEnemy3, 100, 0.2f);

    ////

    //auto textureEnemy4 = ResourceManagers::GetInstance()->GetTexture("RPGmob2.png");
    //m_enemy4 = std::make_shared<Enemy4>();
    //m_enemy4->SetPosition(Vector2(500, 150));
    //m_enemy4->Init2(textureEnemy4, 100, 0.2f);

   
   



    //// Initialize the map
    //m_map = std::make_shared<Map>();
    //if (!m_map->LoadFromFile("Data/Asset/test2/main.tmx", Renderer::GetInstance()->GetRenderer())) {
    //    printf("Failed to load map!\n");
    //    return;
    //}

    // Load background texture
    auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play1.tga");
    m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
    m_background->Set2DPosition(0, 0);

    // Initialize close button
    texture = ResourceManagers::GetInstance()->GetTexture("btn_close.tga");
    button = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
    button->SetSize(50, 50);
    button->Set2DPosition(SCREEN_WIDTH - 50, 10);
    button->SetOnClick([this]() {
        GameStateMachine::GetInstance()->PopState();
        });
    m_listButton.push_back(button);

    // Initialize player
    texture = ResourceManagers::GetInstance()->GetTexture("SpriteSheet_player_9_8.png");
    animation = std::make_shared<SpriteAnimationPlayer>(texture, 9, 8, 0, 0, 30);
    player = std::make_shared<Player>(animation);
    player->SetSize(60, 60);
    player->Set2DPosition(100, 485);
    m_listPlayer.push_back(player);


    m_currentLevel = std::make_shared<Level>(1);
    m_currentLevel->Init(player);
   
   /* m_enemy->SetTargetPlayer(player);
    m_enemy2->SetTargetPlayer(player);*/

    // Set up camera
    Camera::GetInstance()->SetLevelDimension(
        m_currentLevel->GetMap()->GetWidth(),
        m_currentLevel->GetMap()->GetHeight()
    );

    Camera::GetInstance()->SetTarget(player);



}

void GSPlay::Exit() {}

void GSPlay::Pause() {}

void GSPlay::Resume() {}

void GSPlay::HandleEvents() {}

void GSPlay::HandleKeyEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_a:
        case SDLK_LEFT:
            m_KeyPress |= KEY_LEFT;
            break;
        case SDLK_s:
        case SDLK_DOWN:
            m_KeyPress |= KEY_DOWN;
            break;
        case SDLK_d:
        case SDLK_RIGHT:
            m_KeyPress |= KEY_RIGHT;
            break;
        case SDLK_w:
        case SDLK_UP:
            m_KeyPress |= KEY_UP;
            break;
        case SDLK_j:
            m_KeyPress |= KEY_SHOOT;
            break;
        case SDLK_k:
            m_KeyPress |= KEY_JUMP;
            break;
        default:
            break;
        }
    }
    else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        case SDLK_a:
        case SDLK_LEFT:
            m_KeyPress ^= KEY_LEFT;
            break;
        case SDLK_s:
        case SDLK_DOWN:
            m_KeyPress ^= KEY_DOWN;
            break;
        case SDLK_d:
        case SDLK_RIGHT:
            m_KeyPress ^= KEY_RIGHT;
            break;
        case SDLK_w:
        case SDLK_UP:
            m_KeyPress ^= KEY_UP;
            break;
        case SDLK_j:
            m_KeyPress ^= KEY_SHOOT;
            break;
        case SDLK_k:
            m_KeyPress ^= KEY_JUMP;
            break;
        default:
            break;
        }
    }
}

void GSPlay::HandleTouchEvents(SDL_Event& e) {
    for (auto button : m_listButton) {
        if (button->HandleTouchEvent(&e)) {
            break;
        }
    }
}

void GSPlay::HandleMouseMoveEvents(int x, int y) {}

void GSPlay::Update(float deltaTime) {
    // Update key press events
    for (auto it : m_listPlayer) {
        it->HandleInput(m_KeyPress);
    }
	//printf("KeyPress: %d\n", m_KeyPress);

    // Update map
   /* m_map->Update(deltaTime);*/

    // Update buttons
    for (auto it : m_listButton) {
        it->Update(deltaTime);
    }

    // Update animations
    for (auto it : m_listAnimation) {
        it->Update(deltaTime);
    }

    //// Update players
    //for (auto it : m_listPlayer) {
    //    it->Update(deltaTime);
    //}
    /*m_enemy->Update(deltaTime);
    m_enemy2->Update(deltaTime);
    m_enemy3->Update(deltaTime);
    m_enemy4->Update(deltaTime);*/
    //printf("enemy %d %d\n", m_enemy->GetPosition().x, m_enemy->GetPosition().y);
    m_currentLevel->Update(deltaTime);

    // Update camera
    Camera::GetInstance()->Update(deltaTime);

    
    int lvState = LevelStates::GetInstance()->GetLevelState();
    /*if (lvState == 2) {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_COMPLETE);
    }*/
}

void GSPlay::Draw(SDL_Renderer* renderer) {
    
    for (auto it : m_listButton) {
        it->Draw(renderer);
    }

    // Draw animations
    for (auto it : m_listAnimation) {
        it->Draw(renderer);
    }

    //// Draw players
    //for (auto it : m_listPlayer) {
    //    it->Draw(renderer);
    //}

    /*m_enemy->Draw(renderer);
    m_enemy2->Draw(renderer);
    m_enemy3->Draw(renderer);
    m_enemy4->Draw(renderer);*/
    m_currentLevel->Draw(renderer);
}
