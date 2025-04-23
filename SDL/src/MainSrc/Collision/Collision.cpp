#include "Collision.h"

bool Collision::CheckAABB(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}

bool Collision::CheckAABB(const SDL_Rect& a, const std::vector<SDL_Rect>& others) {
    for (const auto& b : others) {
        if (SDL_HasIntersection(&a, &b)) return true;
    }
    return false;
}

SDL_Point Collision::GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle) {
    SDL_Point push = { 0, 0 };

    int dx1 = obstacle.x + obstacle.w - moving.x;     // from left
    int dx2 = moving.x + moving.w - obstacle.x;       // from right
    int dy1 = obstacle.y + obstacle.h - moving.y;     // from top
    int dy2 = moving.y + moving.h - obstacle.y;       // from bottom

    if (dx1 < dx2) push.x = dx1;
    else push.x = -dx2;

    if (dy1 < dy2) push.y = dy1;
    else push.y = -dy2;

    if (abs(push.x) < abs(push.y)) push.y = 0;
    else push.x = 0;

    return push;
}

bool Collision::IsOnGround(const SDL_Rect& player, const SDL_Rect& tile) {
    int threshold = 2;
    return (player.y + player.h <= tile.y + threshold && player.y + player.h >= tile.y - threshold) &&
        (player.x + player.w > tile.x && player.x < tile.x + tile.w);
}
