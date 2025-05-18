#pragma once
#include "GameStateBase.h"
#include "Text.h"
#include <vector>
#include "MouseButton.h"
#include "Sprite2D.h"
class Credit : public GameStateBase {
public:
	Credit();
	~Credit();
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
	std::shared_ptr<Text> m_name1;
	std::shared_ptr<Text> m_name2;
	std::shared_ptr<Text> m_ReferenceText;
	std::shared_ptr<Text> m_text;
	std::vector < std::shared_ptr<MouseButton>> m_listBtn;
	std::shared_ptr<Text> m_newText1;
	std::shared_ptr<Text> m_newText2;
	std::shared_ptr<Text> m_TextForMove;
	std::shared_ptr<Text> m_TextForShoot;
	std::shared_ptr<Text> m_TextForJump;
};