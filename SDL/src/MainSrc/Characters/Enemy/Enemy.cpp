#include "Enemy.h"

Enemy::Enemy(std::shared_ptr<SpriteAnimationPlayer> sprite) :
	Character(sprite)
{
}

Enemy::~Enemy()
{
}



// Getters
float Enemy::GetSightRange() const
{
	return m_sightRange;
}

float Enemy::GetShootRange() const
{
	return m_shootRange;
}

std::shared_ptr<Character> Enemy::GetTarget() const
{
	return m_target;
}

// Setters
void Enemy::SetSightRange(float range)
{
	m_sightRange = range;
}

void Enemy::SetShootRange(float range)
{
	m_shootRange = range;
}

void Enemy::SetTarget(std::shared_ptr<Character> target)
{
	m_target = target;
}