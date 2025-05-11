#include "Level.h"
#include "LevelStates.h"
#include "ResourceManagers.h"
#include "SDL.h"
Level::Level(int level)
    : m_level(level)
    {
}


Level::~Level() {
   
}
void Level::Init(std::shared_ptr<Player> p) {
    m_player = p;
    int lvState = LevelStates::GetInstance()->GetLevelState();
    if (lvState == 1) {
        // init map 1
        m_map = std::make_shared<Map>();
        if (!m_map->LoadFromFile("Data/Asset/test/main.tmx", Renderer::GetInstance()->GetRenderer())) {
            printf("Failed to load map!\n");
            return;
        }

        // init enemy
            auto textureEnemy = ResourceManagers::GetInstance()->GetTexture("ARMob.png");
            auto m_enemy1 = std::make_shared<Enemy>();
            m_enemy1->SetPosition(Vector2(500, 495));
            m_enemy1->SetFPositon(Vector2(500, 495));
            m_enemy1->Init2(textureEnemy, 100, 0.2f);
            m_enemy1->SetTargetPlayer(m_player);
            m_listEnemy1.push_back(m_enemy1);


            auto textureEnemy2 = ResourceManagers::GetInstance()->GetTexture("sprite.png");
            auto m_enemy2 = std::make_shared<Enemy2>();
            m_enemy2->SetPosition(Vector2(600, 95));
            m_enemy2->SetFposition(Vector2(600, 95));
            m_enemy2->Init2(textureEnemy2, 100, 0.2f);
            m_enemy2->SetTargetPlayer(m_player);
            m_listEnemy2.push_back(m_enemy2);
            






    }
    if (lvState == 2) {
        m_listEnemy1.clear();
        m_listEnemy2.clear();
        // init map 1
        m_map = std::make_shared<Map>();
        if (!m_map->LoadFromFile("Data/Asset/test2/main.tmx", Renderer::GetInstance()->GetRenderer())) {
            printf("Failed to load map!\n");
            return;
        }

        // init enemy


        auto textureEnemy = ResourceManagers::GetInstance()->GetTexture("ARMob.png");
        auto m_enemy1 = std::make_shared<Enemy>();
        m_enemy1->SetPosition(Vector2(600, 495));
        m_enemy1->SetFPositon(Vector2(600, 495));
        m_enemy1->Init2(textureEnemy, 100, 0.2f);
        m_enemy1->SetTargetPlayer(m_player);
        m_listEnemy1.push_back(m_enemy1);


        auto textureEnemy2 = ResourceManagers::GetInstance()->GetTexture("sprite.png");
        auto m_enemy2 = std::make_shared<Enemy2>();
        m_enemy2->SetPosition(Vector2(600, 95));
        m_enemy2->SetFposition(Vector2(600, 95));
        m_enemy2->Init2(textureEnemy2, 100, 0.2f);
        m_enemy2->SetTargetPlayer(m_player);
        m_listEnemy2.push_back(m_enemy2);






    }
    if (lvState == 3) {

        m_listEnemy1.clear();
        m_listEnemy2.clear();
        m_map = std::make_shared<Map>();
        if (!m_map->LoadFromFile("Data/Asset/test3/main.tmx", Renderer::GetInstance()->GetRenderer())) {
            printf("Failed to load map!\n");
            return;
        }

        // init enemy
        auto textureEnemy = ResourceManagers::GetInstance()->GetTexture("ARMob.png");
        auto m_enemy1 = std::make_shared<Enemy>();
        m_enemy1->SetPosition(Vector2(500, 495));
        m_enemy1->SetFPositon(Vector2(500, 495));
        m_enemy1->Init2(textureEnemy, 100, 0.2f);
        m_enemy1->SetTargetPlayer(m_player);
        m_listEnemy1.push_back(m_enemy1);


        auto textureEnemy2 = ResourceManagers::GetInstance()->GetTexture("sprite.png");
        auto m_enemy2 = std::make_shared<Enemy2>();
        m_enemy2->SetPosition(Vector2(600, 95));
        m_enemy2->SetFposition(Vector2(600, 95));
        m_enemy2->Init2(textureEnemy2, 100, 0.2f);
        m_enemy2->SetTargetPlayer(m_player);
        m_listEnemy2.push_back(m_enemy2);
    }
}

void Level::Update(float deltaTime) {
    m_map->Update( deltaTime);
    for (auto &e1 : m_listEnemy1) {
        e1->Update(deltaTime);
    }
    for (auto& e2 : m_listEnemy2) {
        e2->Update(deltaTime);
    }
    for (auto& e3 : m_listEnemy3) {
        e3->Update(deltaTime);
    }
    for (auto& e4 : m_listEnemy4) {
        e4->Update(deltaTime);
    }
    m_player->Update(deltaTime);
    isComplete();
}

void Level::Draw(SDL_Renderer* renderer, SDL_Rect* clip) {
    m_map->Draw(renderer);

    for (auto& e1 : m_listEnemy1) {
        e1->Draw(renderer);
    }
    for (auto& e2 : m_listEnemy2) {
        e2->Draw(renderer);
    }
    for (auto& e3 : m_listEnemy3) {
        e3->Draw(renderer);
    }
    for (auto& e4 : m_listEnemy4) {


        e4->Draw(renderer);
    }
    m_player->Draw(renderer);
}

void Level::isComplete() {
   
    if (m_player && m_player->GetPosition().x >= m_map->GetWidth() - 100) {
       
        int currentLevelState = LevelStates::GetInstance()->GetLevelState();
        LevelStates::GetInstance()->SetLevelState(currentLevelState + 1);
        Init(m_player);
        Camera::GetInstance()->SetLevelDimension(
            m_map->GetWidth(),
            m_map->GetHeight()
        );
       

       
        m_player->Set2DPosition(100, 485);
    }
}

