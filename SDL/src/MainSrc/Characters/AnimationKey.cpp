#include "AnimationKey.h"
bool AnimationKey::operator == (const AnimationKey& other) const {
	return isOnGround == other.isOnGround && isShooting == other.isShooting && directionGun == other.directionGun;
}
