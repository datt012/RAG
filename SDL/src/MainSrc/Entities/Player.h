#include "SDL.h"
#include "GameObject/BaseObject.h"
#include "GameManager/Singleton.h"
#include "SpriteAnimation.h"
class Player : public BaseObject {
public:
	void init();
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer, SDL_Rect* clip = NULL) override;
	Player();
private:
	std::shared_ptr<SpriteAnimation> idleAnimation;// đứng yên
	std::shared_ptr<SpriteAnimation> walkAnimation;
	std::shared_ptr<SpriteAnimation> jumpAnimation;
	std::shared_ptr<SpriteAnimation> attackAnimation;
	std::shared_ptr<SpriteAnimation> fallAnimation;
	std::shared_ptr<SpriteAnimation> hitAnimation;
	std::shared_ptr<SpriteAnimation> dieAnimation;

	int status;//1idle 2walk 3jump 4attack 5fall 6hit 7die;
	int health;
	int speed;

	bool playerMoveLeft;
	bool playerMoveRight;
	bool playerJump;
	bool playerHit;
	bool playerIdle;
	bool playerWalk;
	bool playerDie;
	bool playerAttack;




};