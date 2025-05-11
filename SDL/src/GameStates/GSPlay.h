#pragma once
#include "GameStateBase.h"
#include "MainSrc/Map/Map.h"
#include "ResourceManagers.h"
#include "GameObject/MouseButton.h"
#include "GameObject/Sprite2D.h"
#include "GameObject/SpriteAnimation.h"
#include "GameObject/Camera.h"
#include "KeyState.h"
#include "Sound.h"
#include "MainSrc/Characters/Player.h"
#include "MainSrc/Characters/KeyDirection.h"
#include "MainSrc/Entities/Enemy.h"
#include "MainSrc/Entities/Enemy2.h"
#include "MainSrc/Entities/Enemy3.h"
#include "MainSrc/Entities/Enemy4.h"
#include "MainSrc/LevelDesign/Level.h"

class Sprite2D;
class SpriteAnimation;
class MouseButton;
class Sound;

class GSPlay : public GameStateBase {
public:
    GSPlay();
    ~GSPlay();

    void Init() override;
    void Exit() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents() override;
    void HandleKeyEvents(SDL_Event& e) override;
    void HandleTouchEvents(SDL_Event& e) override;
    void HandleMouseMoveEvents(int x, int y) override;
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;

private:
    std::shared_ptr<Sprite2D> m_background;
    std::shared_ptr<SpriteAnimation> obj;
    std::shared_ptr<SpriteAnimationPlayer> animation;
    std::shared_ptr<Player> player;
    std::shared_ptr<MouseButton> button;
    std::shared_ptr<Map> m_map; 

    std::list<std::shared_ptr<MouseButton>> m_listButton;
    std::list<std::shared_ptr<SpriteAnimation>> m_listAnimation;
    std::list<std::shared_ptr<Player>> m_listPlayer;


    /*std::shared_ptr<Enemy> m_enemy;
    std::shared_ptr<Enemy2> m_enemy2;
    std::shared_ptr<Enemy3> m_enemy3;
    std::shared_ptr<Enemy4> m_enemy4;*/

    std::shared_ptr<Level> m_currentLevel;

    int m_KeyPress;
    float time = 0.0f;
    float m_Velocity = 10.0f;
};
