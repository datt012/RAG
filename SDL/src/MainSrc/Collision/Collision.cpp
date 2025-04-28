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

Vector2 Collision::GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle) {
    Vector2 push = { 0, 0 };

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

Vector2 Collision::GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle, const Vector2& velocity) {
    Vector2 push(0, 0);

    int dx1 = obstacle.x + obstacle.w - moving.x;     // from right
    int dx2 = moving.x + moving.w - obstacle.x;       // from left
    int dy1 = obstacle.y + obstacle.h - moving.y;     // from bottom
    int dy2 = moving.y + moving.h - obstacle.y;       // from top

    if (velocity.x < 0) {
        push.x = dx1;  // from right
    }
    else if (velocity.x > 0) {
        push.x = -dx2; // from left
    }

    if (velocity.y < 0) {
        push.y = dy1;  // from bottom
    }
    else if (velocity.y > 0) {
        push.y = -dy2; // from top
    }

    Vector2 step;
    step.x = (abs(push.x) + abs(velocity.x)) / abs(velocity.x);
    step.y = (abs(push.y) + abs(velocity.y)) / abs(velocity.y);
     
    if (step.x < step.y) {
        push.y = 0;
    }
    else {
        push.x = 0;
    }

    return push;
}

Vector2 Collision::GetPushVector(const SDL_Rect& moving, const std::vector<SDL_Rect>& obstacles, const Vector2& velocity) {
    SDL_Rect rect = { moving.x - velocity.x, moving.y - velocity.y, moving.w, moving.h };
    float x_step = velocity.x / std::min(abs(velocity.x), abs(velocity.y));
    float y_step = velocity.y / std::min(abs(velocity.x), abs(velocity.y));

    bool x_increase = x_step;
    bool y_increase = y_step;

    while ((x_increase || y_increase) && rect.x * (velocity.x > 0 ? 1 : -1) < moving.x * (velocity.x > 0 ? 1 : -1) &&
        rect.y * (velocity.y > 0 ? 1 : -1) < moving.y * (velocity.y > 0 ? 1 : -1)) {

        if (x_increase) {
            rect.x += x_step;
            if (CheckAABB(rect, obstacles)) {
                x_increase = false;
                rect.x -= x_step;
            }
        }

        if (y_increase) {
            rect.y += y_step;
            if (CheckAABB(rect, obstacles)) {
                y_increase = false;
                rect.y -= y_step;
            }
        }
    }

    if (rect.x == moving.x) {
        rect.x -= x_step;
    }

    if (rect.y == moving.y) {
        rect.y -= y_step;
    }

    return Vector2(rect.x - moving.x, rect.y - moving.y);
}

bool Collision::IsOnGround(const SDL_Rect& player, const SDL_Rect& tile) {
    int threshold = 2;
    return (player.y + player.h <= tile.y + threshold && player.y + player.h >= tile.y - threshold) &&
        (player.x + player.w > tile.x && player.x < tile.x + tile.w);
}
