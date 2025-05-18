#include "GSPlay.h"
#include "MainSrc/Level/Level.h"
#include "GSMenu.h"
#include "Sound.h"
GSPlay::GSPlay() : m_KeyPress(0) {}
GSPlay::~GSPlay() {}
void GSPlay::Init() {
    m_map = std::make_shared<Map>();
    if (!m_map->LoadFromFile("Data/Asset/Map1/main.tmx", Renderer::GetInstance()->GetRenderer())) {
        printf("Failed to load map!\n");
        return;
    }
    auto texture = ResourceManagers::GetInstance()->GetTexture("btn_play.png");
    m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
    m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
    m_background->Set2DPosition(0, 0);
    texture = ResourceManagers::GetInstance()->GetTexture("btn_close.png");
    button = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
    button->SetSize(50, 50);
    button->Set2DPosition(SCREEN_WIDTH - 50, 10);
    button->SetOnClick([this]() {
        GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
    });
    m_listButton.push_back(button);
    auto texture2 = ResourceManagers::GetInstance()->GetTexture("btn_pause.png");
    button = std::make_shared<MouseButton>(texture2, SDL_FLIP_NONE);
    button->SetSize(50, 50);
    button->Set2DPosition(SCREEN_WIDTH - 100, 10);
    button->SetOnClick([this]() {
        GSMenu::SetPauseFlag(true);
        GameStateMachine::GetInstance()->PushState(StateType::STATE_MENU);
    });
    m_listButton.push_back(button);
    texture = ResourceManagers::GetInstance()->GetTexture(PLAYER_SPRITE_PATH);
    animation = std::make_shared<SpriteAnimationPlayer>(texture, 12, 8, 0, 0, 30);
    m_player = std::make_shared<Player>(animation);
    m_player->SetSize(PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT);
    m_player->Set2DPosition(150, 0);
    m_player->Init();
    Init2(m_player);
    Camera::GetInstance()->SetLevelDimension(m_map->GetWidth(), m_map->GetHeight());
    Camera::GetInstance()->SetTarget(m_player);
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
        if (button->HandleTouchEvent(&e)) break;
    }
}
void GSPlay::HandleMouseMoveEvents(int x, int y) {}
void GSPlay::Update(float deltaTime) {
    if (deltaTime > 200) return;
    m_player->HandleInput(m_KeyPress);
    for (auto it : m_listEnemy) {
        it->HandleInput(Behavior::GenerateKeyMask(it, m_map));
    }
    if (Level::GetInstance()->GetLevel() == 3) {
        bool mobDead = false;
        for (auto it : m_listEnemy) {
            if (!it->IsAlive() && it->GetMAXHP() == BOSS1_MAX_HP) {
                if (countDownDeadAll <= 0) {
                    for (auto it : m_listEnemy) {
                        it->SetHP(it->GetHP() - 1);
                    }
                }
                else {
                    countDownDeadAll -= static_cast<int>(deltaTime);
                }
            }
        }
        for (auto it : m_listEnemy) {
            if (!it->IsAlive() && it->GetMAXHP() != BOSS1_MAX_HP) {
                mobDead = true;
                break;
            }
        }
        if (mobDead) {
            if (m_revivalTimeCountDown <= 0) {
                for (auto it : m_listEnemy) {
                    if (!it->IsAlive() && it->GetMAXHP() < BOSS1_MAX_HP) {
                        it->SetHP(it->GetMAXHP());
                        it->SetShootCooldown(it->GetShootCooldownTime());
                        auto p = it->Get2DPosition();
                        it->Set2DPosition(p.x, p.y - it->GetHeight() - 2);
                    }
                }
                m_revivalTimeCountDown = m_revivalTime;
            }
            else {
                m_revivalTimeCountDown -= static_cast<int>(deltaTime);
            }
        }
    }
    m_map->Update(deltaTime);
    for (auto it : m_listButton) {
        it->Update(deltaTime);
    }
    for (auto it : m_listAnimation) {
        it->Update(deltaTime);
    }
    m_player->Update(deltaTime);
    m_player->SolveCollision(m_map);
    if (m_player->GetHP() <= 0) {
        if (countDownGameOver <= 0) {
            GameStateMachine::GetInstance()->ChangeState(StateType::STATE_OVER);
        }
        else {
            countDownGameOver -= static_cast<int>(deltaTime);
        }
    }
    for (auto it : m_listEnemy) {
        it->Update(deltaTime);
        it->SolveCollision(m_map);
    }
    auto playerBulletPool = m_player->GetBulletPool();
    if (playerBulletPool) {
        playerBulletPool->Update(deltaTime);
        for (auto bullet : playerBulletPool->GetBullets()) {
            if (bullet->IsActive()) {
                SDL_FRect rectBullet = bullet->getColliderFRect();
                for (auto enemy : m_listEnemy) {
                    if (enemy->IsAlive()) {
                        if (Collision::CheckAABB(rectBullet, enemy->GetColliderFRect())) {
                            enemy->TakeDamage(bullet->GetDamage());
                            bullet->Deactivate();
                        }
                    }
                }
            }
        }
        playerBulletPool->SolveCollision(m_map);
    }
    for (auto it : m_listEnemy) {
        auto bulletPool = it->GetBulletPool();
        if (bulletPool) {
            bulletPool->Update(deltaTime);
            for (auto bullet : bulletPool->GetBullets()) {
                if (bullet->IsActive()) {
                    SDL_FRect rectBullet = bullet->getColliderFRect();
                    if (m_player->IsAlive()) {
                        if (Collision::CheckAABB(rectBullet, m_player->GetColliderFRect())) {
                            m_player->TakeDamage(bullet->GetDamage());
                            bullet->Deactivate();
                        }
                    }
                }
            }
            bulletPool->SolveCollision(m_map);
        }
    }
    Camera::GetInstance()->Update(deltaTime);
    IsComplete(deltaTime);
}
void GSPlay::Draw(SDL_Renderer* renderer) {
    m_map->Draw(renderer);
    for (auto it : m_listAnimation) {
        it->Draw(renderer);
    }
    m_player->Draw(renderer);
    for (auto it : m_listEnemy) {
        it->Draw(renderer);
    }
    auto playerBulletPool = m_player->GetBulletPool();
    if (playerBulletPool) {
        playerBulletPool->Draw(renderer);
    }
    for (auto it : m_listEnemy) {
        auto bulletPool = it->GetBulletPool();
        if (bulletPool) {
            bulletPool->Draw(renderer);
        }
    }
    for (auto it : m_listButton) {
        it->Draw(renderer);
    }
}
void GSPlay::Init2(std::shared_ptr<Player> p) {
    m_player = p;
    int lv = Level::GetInstance()->GetLevel();
    std::vector<Vector2> armobPositions;
    std::vector<Vector2> sniperMobPositions;
    std::vector<Vector2> rpgMobPositions;
    std::vector<Vector2> boss1Positions;
    Vector2 initPosition;
    if (lv == 1) {
        m_listEnemy.clear();
        m_map = std::make_shared<Map>();
        if (!m_map->LoadFromFile("Data/Asset/Map1/main.tmx", Renderer::GetInstance()->GetRenderer())) {
            printf("Failed to load map!\n");
            return;
        }
        initPosition = { 150, 100 };
        armobPositions = {
            {300, 0},
            {500, 0},
            {698, 0},
            {800, 0},
            {1100, 0},
            {1300, 0},
            {1500, 0},
            {1800, 0},
        };
        sniperMobPositions = {};
        rpgMobPositions = {};
        boss1Positions = {};
    }
    if (lv == 2) {
        m_map = NULL;
        m_listEnemy.clear();
        m_map = std::make_shared<Map>();
        if (!m_map->LoadFromFile("Data/Asset/Map2/main.tmx", Renderer::GetInstance()->GetRenderer())) {
            printf("Failed to load map!\n");
            return;
        }
        initPosition = { 150, 110 };
        armobPositions = {
            {1567, 0},
            {878, 154},
            {782, 320},
            {1232, 444},
            {1753, 511},
        };
        sniperMobPositions = {
            {680, 90},
            {800, 36},
            {1040, 60},
            {1602, 0},
            {1659, 0},
            {111, 453},
            {449, 422},
            {1550, 511},
        };
        rpgMobPositions = {};
        boss1Positions = {};
    }
    if (lv == 3) {
        m_listEnemy.clear();
        m_map = std::make_shared<Map>();
        if (!m_map->LoadFromFile("Data/Asset/Map3/main.tmx", Renderer::GetInstance()->GetRenderer())) {
            printf("Failed to load map!\n");
            return;
        }
        initPosition = { 740, 390 };
        armobPositions = {};
        sniperMobPositions = {};
        rpgMobPositions = {
            {129, 256},
            {400, 320},
            {628, 384},
            {740, 390},
            {818, 390},
            {932, 384},
            {1156, 320},
            {1416, 256},
        };
        boss1Positions = {
            {900, 30},
        };
    }
    for (const auto& pos : armobPositions) {
        auto texture = ResourceManagers::GetInstance()->GetTexture(ARMOB_SPRITE_PATH);
        animation = std::make_shared<SpriteAnimationPlayer>(texture, 1, 26, 0, 0, 30);
        enemy = std::make_shared<ARMob>(animation);
        enemy->SetSize(ARMOB_SIZE_WIDTH, ARMOB_SIZE_HEIGHT);
        enemy->Set2DPosition(pos.x, pos.y);
        enemy->SetTarget(m_player);
        enemy->Init();
        m_listEnemy.push_back(enemy);
    }
    for (const auto& pos : sniperMobPositions) {
        auto texture = ResourceManagers::GetInstance()->GetTexture(SNIPER_SPRITE_PATH);
        animation = std::make_shared<SpriteAnimationPlayer>(texture, 1, 14, 0, 0, 30);
        enemy = std::make_shared<SniperMob>(animation);
        enemy->SetSize(SNIPER_SIZE_WIDTH, SNIPER_SIZE_HEIGHT);
        enemy->Set2DPosition(pos.x, pos.y);
        enemy->SetTarget(m_player);
        enemy->Init();
        m_listEnemy.push_back(enemy);
    }
    for (const auto& pos : rpgMobPositions) {
        auto texture = ResourceManagers::GetInstance()->GetTexture(RPGMOB_SPRITE_PATH);
        animation = std::make_shared<SpriteAnimationPlayer>(texture, 1, 11, 0, 0, 30);
        enemy = std::make_shared<RPGMob>(animation);
        enemy->SetSize(RPGMOB_SIZE_WIDTH, RPGMOB_SIZE_HEIGHT);
        enemy->Set2DPosition(pos.x, pos.y);
        enemy->SetTarget(m_player);
        enemy->Init();
        enemy->SetHP(0);
        m_listEnemy.push_back(enemy);
    }
    for (const auto& pos : boss1Positions) { 
        auto texture = ResourceManagers::GetInstance()->GetTexture(BOSS1_SPRITE_PATH);
        animation = std::make_shared<SpriteAnimationPlayer>(texture, 1, 7, 0, 0, 30);
        enemy = std::make_shared<Boss1>(animation);
        enemy->SetSize(BOSS1_SIZE_WIDTH, BOSS1_SIZE_HEIGHT);
        enemy->Set2DPosition(pos.x, pos.y);
        enemy->SetTarget(m_player);
        enemy->Init();
        m_listEnemy.push_back(enemy);
    }
    m_player->Set2DPosition(initPosition.x, initPosition.y);
    m_revivalTimeCountDown = 10000;
    countDownDeadAll = 3000;
    countDownComplete = 5000;
    countDownGameOver = 3000;
}
void GSPlay::DeactivateBullets() {
    std::shared_ptr<BulletPool> bulletPool;
    bulletPool = m_player->GetBulletPool();
    for (auto& bullet : bulletPool->GetBullets()) {
        bullet->Deactivate();
    }
    for (auto it : m_listEnemy) {
        bulletPool = m_player->GetBulletPool();
        for (auto& bullet : bulletPool->GetBullets()) {
            bullet->Deactivate();
        }
    }
}
void GSPlay::IsComplete(float deltatime) {
    if (!m_player || !m_map) return;
    float playerRightEdge = m_player->GetColliderFRect().x + m_player->GetColliderFRect().w;
    float mapWidth = m_map->GetWidth();
    bool allEnemiesDefeated = true;
    for (const auto& enemy : m_listEnemy) {
        if (enemy->IsAlive()) {
            allEnemiesDefeated = false;
            break;
        }
    }
    if (Level::GetInstance()->GetLevel() == 3 && allEnemiesDefeated) {
        if (countDownComplete <= 0) {
            GameStateMachine::GetInstance()->ChangeState(StateType::STATE_COMPLETE);
        }
        else {
            countDownComplete -= deltatime;
        }
    }
    if (playerRightEdge >= mapWidth && allEnemiesDefeated) {
        int currentLevel = Level::GetInstance()->GetLevel();
        Level::GetInstance()->SetLevel(currentLevel + 1);
        m_map = nullptr;
        DeactivateBullets();
        Init2(m_player);
        Camera::GetInstance()->SetLevelDimension(m_map->GetWidth(), m_map->GetHeight());
        m_player->SetHP(m_player->GetHP() + m_player->GetMAXHP());
    }
}