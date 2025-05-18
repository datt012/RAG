#pragma once
#include <algorithm>
#include <memory>
#include "MainSrc/Characters/KeyDirection.h"
#include "MainSrc/Collision/Collision.h"
#include "MainSrc/Characters/Character.h"
#include "MainSrc/Characters/AnimationKey.h"
class Enemy : public Character {
public:
	Enemy(std::shared_ptr<SpriteAnimationPlayer> sprite);
	~Enemy();
	float GetSightRange() const;
	float GetShootRange() const;
	std::shared_ptr<Character> GetTarget() const;
	void SetSightRange(float range);
	void SetShootRange(float range);
	void SetTarget(std::shared_ptr<Character> target);
protected:
	float m_sightRange;
	float m_shootRange;
	std::shared_ptr<Character> m_target;
};