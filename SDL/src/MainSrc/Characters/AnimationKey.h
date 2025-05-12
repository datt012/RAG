#pragma once

#pragma once
#include <functional>
#include "DirectionGun.h"

struct AnimationKey {
    bool isOnGround;
    bool isShooting;
	DirectionGun directionGun;

    bool operator==(const AnimationKey& other) const;
};

namespace std {
    template <>
    struct hash<AnimationKey> {
        size_t operator()(const AnimationKey& k) const {
            size_t h1 = std::hash<bool>()(k.isOnGround);
            size_t h2 = std::hash<bool>()(k.isShooting);
            size_t h3 = std::hash<int>()(static_cast<int>(k.directionGun));

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}