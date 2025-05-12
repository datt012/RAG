#include "GSPlay.h"

GSPlay::GSPlay() : m_KeyPress(0) {}

GSPlay::~GSPlay() {}

void GSPlay::Init() {
    // Initialize the map
    m_map = std::make_shared<Map>();
    if (!m_map->LoadFromFile("Data/Asset/test/main2.tmx", Renderer::GetInstance()->GetRenderer())) {
        printf("Failed to load map!\n");
        return;
    }

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
    texture = ResourceManagers::GetInstance()->GetTexture(PLAYER_SPRITE_PATH);
    animation = std::make_shared<SpriteAnimationPlayer>(texture, 12, 8, 0, 0, 30);
    player = std::make_shared<Player>(animation);
    player->SetSize(PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT);
    player->Set2DPosition(150, 0);
    player->Init();
    m_listPlayer.push_back(player);

    // Initialize enemy
    
	std::vector<Vector2> armobPositions = {
        {300, 0},
        {500, 0},
        {700, 0},
        {800, 0},
        {1100, 0},
        {1300, 0},
        {1500, 0},
        {1800, 0},
	};
    texture = ResourceManagers::GetInstance()->GetTexture(ARMOB_SPRITE_PATH);
	for (const auto& pos : armobPositions) {
        animation = std::make_shared<SpriteAnimationPlayer>(texture, 1, 26, 0, 0, 30);
		enemy = std::make_shared<ARMob>(animation);
		enemy->SetSize(ARMOB_SIZE_WIDTH, ARMOB_SIZE_HEIGHT);
		enemy->Set2DPosition(pos.x, pos.y);
		enemy->SetTarget(player);
		enemy->Init();
		m_listEnemy.push_back(enemy);
	}

    // Set up camera
    Camera::GetInstance()->SetLevelDimension(m_map->GetWidth(), m_map->GetHeight());
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
    if (deltaTime > 200) return;

    // Update key press events
    for (auto it : m_listPlayer) {
        it->HandleInput(m_KeyPress);
    }
	printf("KeyPress: %d\n", m_KeyPress);

    for (auto it : m_listEnemy) {
        it->HandleInput(Behavior::GenerateKeyMask(it, m_map));
    }

    // Update map
    m_map->Update(deltaTime);

    // Update buttons
    for (auto it : m_listButton) {
        it->Update(deltaTime);
    }

    // Update animations
    for (auto it : m_listAnimation) {
        it->Update(deltaTime);
    }

    // Update players
    for (auto it : m_listPlayer) {
        it->Update(deltaTime);
        it->SolveCollision(m_map);
		printf("hp player : %d\n", it->GetHP());
    }

    // Update enemy
	for (auto it : m_listEnemy) {
		it->Update(deltaTime);
        it->SolveCollision(m_map);
		printf("hp : %d\n", it->GetHP());
	}

	// Update bullets
    for (auto it : m_listPlayer) {
		auto bulletPool = it->GetBulletPool();
		if (bulletPool) {
			bulletPool->Update(deltaTime);

			for (auto bullet : bulletPool->GetBullets()) {
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

			bulletPool->SolveCollision(m_map);
		}
	}
    for (auto it : m_listEnemy) {
        auto bulletPool = it->GetBulletPool();
        if (bulletPool) {
            bulletPool->Update(deltaTime);

			for (auto bullet : bulletPool->GetBullets()) {
				if (bullet->IsActive()) {
					SDL_FRect rectBullet = bullet->getColliderFRect();
					for (auto player : m_listPlayer) {
						if (player->IsAlive()) {
							if (Collision::CheckAABB(rectBullet, player->GetColliderFRect())) {
								player->TakeDamage(bullet->GetDamage());
								bullet->Deactivate();
							}
						}
					}
				}
			}

			bulletPool->SolveCollision(m_map);
        }
    }

    // Update camera
    Camera::GetInstance()->Update(deltaTime);
}

void GSPlay::Draw(SDL_Renderer* renderer) {
    // Draw background
    //m_background->Draw(renderer);

    // Draw map
    m_map->Draw(renderer);

    // Draw buttons
    for (auto it : m_listButton) {
        it->Draw(renderer);
    }

    // Draw animations
    for (auto it : m_listAnimation) {
        it->Draw(renderer);
    }

    // Draw players
    for (auto it : m_listPlayer) {
        it->Draw(renderer);
    }

    // Draw enemies
	for (auto it : m_listEnemy) {
		it->Draw(renderer);
	}

    // Draw bullets
    for (auto it : m_listPlayer) {
        auto bulletPool = it->GetBulletPool();
        if (bulletPool) {
            bulletPool->Draw(renderer);
        }
    }
    for (auto it : m_listEnemy) {
        auto bulletPool = it->GetBulletPool();
        if (bulletPool) {
            bulletPool->Draw(renderer);
        }
    }
}
