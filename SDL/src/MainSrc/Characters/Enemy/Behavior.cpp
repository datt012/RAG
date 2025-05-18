#include "Behavior.h"
int Behavior::GenerateKeyMask(std::shared_ptr<Enemy> enemy, std::shared_ptr<Map> map) {
	if (!enemy || !map || !enemy->GetTarget()) {
		return 0;
	}
	if (!enemy->GetTarget()->IsAlive()) {
		return 0;
	}
	if (!enemy->IsAlive()) {
		return 0;
	}
	if (auto armob = std::dynamic_pointer_cast<ARMob>(enemy)) {
		return GenerateKeyMask(armob, map);
	}
	else if (auto sniperMob = std::dynamic_pointer_cast<SniperMob>(enemy)) {
		return GenerateKeyMask(sniperMob, map);
	}
	else if (auto rpgMob = std::dynamic_pointer_cast<RPGMob>(enemy)) {
		return GenerateKeyMask(rpgMob, map);
	}
	else if (auto boss1 = std::dynamic_pointer_cast<Boss1>(enemy)) {
		return GenerateKeyMask(boss1, map);
	}
	return 0;
}
bool isFalling(std::shared_ptr<Enemy> enemy, std::shared_ptr<Map> map, const int& keyMask) {
	bool result = false;
	bool isOnGround = enemy->IsOnGround();
	Vector2 enemyPos = enemy->Get2DPosition();
	float epsilon = 1.0f;
	if (keyMask & KEY_RIGHT) {
		enemy->Set2DPosition(enemyPos.x + enemy->GetColliderFRect().w + epsilon, enemyPos.y);
	}
	else if (keyMask & KEY_LEFT) {
		enemy->Set2DPosition(enemyPos.x - enemy->GetColliderFRect().w - epsilon, enemyPos.y);
	}
	enemy->UpdateIsOnGround(map);
	if (!enemy->IsOnGround()) {
		result = true;
	}
	enemy->SetOnGround(isOnGround);
	enemy->Set2DPosition(enemyPos.x, enemyPos.y);
	return result;
}
bool HasCollision(std::shared_ptr<Enemy> enemy, std::shared_ptr<Map> map, const int& keyMask) {
	bool result = false;
	Vector2 enemyPos = enemy->Get2DPosition();
	float epsilon = 1.0f;
	if (keyMask & KEY_RIGHT) {
		enemy->Set2DPosition(enemyPos.x + epsilon, enemyPos.y);
	}
	else if (keyMask & KEY_LEFT) {
		enemy->Set2DPosition(enemyPos.x - epsilon, enemyPos.y);
	}
	if (enemy->CollisionWithMap(map)) {
		result = true;
	}
	enemy->Set2DPosition(enemyPos.x, enemyPos.y);
	return result;
}
int foo1(std::shared_ptr<Enemy> enemy, std::shared_ptr<Map> map) {
	int keyMask = 0;
	Vector2 enemyPos = enemy->Get2DPosition();
	Vector2 playerPos = enemy->GetTarget()->Get2DPosition();
	Vector2 direction = playerPos - enemyPos;
	float angle = atan2(direction.y, direction.x);
	float distance = direction.Length();
	if (distance < enemy->GetSightRange()) {
		if (angle > -M_PI / 4 && angle < M_PI / 4) {
			keyMask |= KEY_RIGHT;
		}
		else if (abs(angle) > 3 * M_PI / 4) {
			keyMask |= KEY_LEFT;
		}
	}
	if (isFalling(enemy, map, keyMask) || HasCollision(enemy, map, keyMask)) {
		keyMask = 0;
	}
	return keyMask;
}
int foo2(std::shared_ptr<Enemy> enemy, std::shared_ptr<Map> map) {
	int keyMask = 0;
	Vector2 enemyPos = enemy->Get2DPosition();
	Vector2 playerPos = enemy->GetTarget()->Get2DPosition();
	Vector2 direction = playerPos - enemyPos;
	float angle = atan2(direction.y, direction.x);
	float distance = direction.Length();
	if (distance < enemy->GetShootRange()) {
		if (angle > -M_PI / 4 && angle < M_PI / 4) {
			enemy->SetFlip(SDL_FLIP_NONE);
			keyMask |= KEY_SHOOT;
		}
		else if (abs(angle) > 3 * M_PI / 4) {
			enemy->SetFlip(SDL_FLIP_HORIZONTAL);
			keyMask |= KEY_SHOOT;
		}
	}
	return keyMask;
}
int bossMove(std::shared_ptr<Enemy> enemy, std::shared_ptr<Map> map) {
	int keyMask = 0;
	Vector2 enemyPos = enemy->Get2DPosition();
	Vector2 playerPos = enemy->GetTarget()->Get2DPosition();
	float x_center_enemy = enemyPos.x + enemy->GetWidth() / 2.0f;
	float x_center_player = playerPos.x + enemy->GetTarget()->GetWidth() / 2.0f;
	float epsilon = enemy->GetVelocity().x ? 0.5f : enemy->GetWidth() * 0.17f;
	if (x_center_enemy < x_center_player - epsilon) {
		keyMask |= KEY_RIGHT;
	}
	else if (x_center_enemy > x_center_player + epsilon) {
		keyMask |= KEY_LEFT;
	}
	return keyMask;
}
int Behavior::GenerateKeyMask(std::shared_ptr<ARMob> armob, std::shared_ptr<Map> map) {
	int keyMask = 0;
	keyMask |= foo2(armob, map) ? foo2(armob, map) : foo1(armob, map);
	return keyMask;
}
int Behavior::GenerateKeyMask(std::shared_ptr<SniperMob> sniperMob, std::shared_ptr<Map> map) {
	int keyMask = 0;
	keyMask |= foo2(sniperMob, map);
	return keyMask;
}
int Behavior::GenerateKeyMask(std::shared_ptr<RPGMob> rpgMob, std::shared_ptr<Map> map) {
	int keyMask = 0;
	keyMask |= foo2(rpgMob, map);
	return keyMask;
}
int Behavior::GenerateKeyMask(std::shared_ptr<Boss1> boss1, std::shared_ptr<Map> map) {
	int keyMask = 0;
	keyMask |= bossMove(boss1, map);
	keyMask |= KEY_SHOOT;
	return keyMask;
}