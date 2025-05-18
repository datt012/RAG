#include "Collision.h"
bool Collision::CheckAABB(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}
bool Collision::CheckAABB(const SDL_FRect& a, const SDL_Rect& b) {
    SDL_FRect f_b = {
        static_cast<float>(b.x),
        static_cast<float>(b.y),
        static_cast<float>(b.w),
        static_cast<float>(b.h)
    };
    return CheckAABB(a, f_b);
}
bool Collision::CheckAABB(const SDL_Rect& a, const SDL_FRect& b) {
    SDL_FRect f_a = {
        static_cast<float>(a.x),
        static_cast<float>(a.y),
        static_cast<float>(a.w),
        static_cast<float>(a.h)
    };
    return CheckAABB(f_a, b);
}
bool Collision::CheckAABB(const SDL_FRect& a, const SDL_FRect& b) {
    float left = std::max(a.x, b.x);
    float right = std::min(a.x + a.w, b.x + b.w);
    float top = std::max(a.y, b.y);
    float bottom = std::min(a.y + a.h, b.y + b.h);
    bool overlap_x = left < right;
    bool overlap_y = top < bottom;
    return overlap_x && overlap_y;
}
bool Collision::CheckAABB(const SDL_Rect& a, const std::vector<SDL_Rect>& others) {
    for (const auto& b : others) {
        if (CheckAABB(a, b)) return true;
    }
    return false;
}
bool Collision::CheckAABB(const SDL_FRect& a, const std::vector<SDL_Rect>& others) {
    for (const auto& b : others) {
        if (CheckAABB(a, b)) return true;
    }
    return false;
}
bool Collision::CheckAABB(const SDL_Rect& a, const std::vector<SDL_FRect>& others) {
    for (const auto& b : others) {
        if (CheckAABB(a, b)) return true;
    }
    return false;
}
bool Collision::CheckAABB(const SDL_FRect& a, const std::vector<SDL_FRect>& others) {
    for (const auto& b : others) {
        if (CheckAABB(a, b)) return true;
    }
    return false;
}
bool Collision::CheckAABB(const SDL_Rect& a, const std::vector<SDL_Rect>& others, SDL_Rect& collidedRect) {
    for (const auto& b : others) {
        if (CheckAABB(a, b)) {
            collidedRect = b;
            return true;
        }
    }
    return false;
}
bool Collision::CheckAABB(const SDL_FRect& a, const std::vector<SDL_Rect>& others, SDL_Rect& collidedRect) {
    for (const auto& b : others) {
        if (CheckAABB(a, b)) {
            collidedRect = b;
            return true;
        }
    }
    return false;
}
bool Collision::CheckAABB(const SDL_Rect& a, const std::vector<SDL_FRect>& others, SDL_FRect& collidedRect) {
    for (const auto& b : others) {
        if (CheckAABB(a, b)) {
            collidedRect = b;
            return true;
        }
    }
    return false;
}
bool Collision::CheckAABB(const SDL_FRect& a, const std::vector<SDL_FRect>& others, SDL_FRect& collidedRect) {
    for (const auto& b : others) {
        if (CheckAABB(a, b)) {
            collidedRect = b;
            return true;
        }
    }
    return false;
}
Vector2 Collision::GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle) {
    Vector2 push = { 0, 0 };
    int dx1 = obstacle.x + obstacle.w - moving.x;     
    int dx2 = moving.x + moving.w - obstacle.x;       
    int dy1 = obstacle.y + obstacle.h - moving.y;     
    int dy2 = moving.y + moving.h - obstacle.y;    
    if (dx1 < dx2) push.x = dx1;
    else push.x = -dx2;
    if (dy1 < dy2) push.y = dy1;
    else push.y = -dy2;
    if (abs(push.x) < abs(push.y)) push.y = 0;
    else push.x = 0;
    return push;
}
Vector2 Collision::GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle, Vector2 displacement) {
    Vector2 push(0, 0);
    int dx1 = obstacle.x + obstacle.w - moving.x;     
    int dx2 = moving.x + moving.w - obstacle.x;       
    int dy1 = obstacle.y + obstacle.h - moving.y;     
    int dy2 = moving.y + moving.h - obstacle.y;      
    if (displacement.x < 0) {
        push.x = dx1;  
    }
    else if (displacement.x > 0) {
        push.x = -dx2; 
    }
    if (displacement.y < 0) {
        push.y = dy1;  
    }
    else if (displacement.y > 0) {
        push.y = -dy2;
    }
    Vector2 step;
    step.x = (abs(push.x) + abs(displacement.x)) / abs(displacement.x);
    step.y = (abs(push.y) + abs(displacement.y)) / abs(displacement.y);
    if (step.x < step.y) {
        push.y = 0;
    }
    else {
        push.x = 0;
    }
    return push;
}
Vector2 Collision::GetPushVector(const SDL_Rect& moving, const std::vector<SDL_Rect>& obstacles, Vector2 displacement) {
    SDL_Rect rect = { moving.x - displacement.x, moving.y - displacement.y, moving.w, moving.h };
    SDL_Rect collidedRect;
    const float moveStep = 4.0f;
    const float total = displacement.Length();
    const int steps = ceil(total / moveStep);
    float stepX = displacement.x / steps;
    float stepY = displacement.y / steps;
    for (int i = 0; i < steps; ++i) {
        rect.x += static_cast<int>(stepX);
        if (CheckAABB(rect, obstacles, collidedRect)) {
            int leftSide = collidedRect.x - rect.w;
            int rightSide = collidedRect.x + collidedRect.w;
            rect.x = stepX > 0 ? leftSide : rightSide;
            stepX = 0; 
        }
        rect.y += static_cast<int>(stepY);
        if (CheckAABB(rect, obstacles, collidedRect)) {
            int topSide = collidedRect.y - rect.h;
            int bottomSide = collidedRect.y + collidedRect.h;
            rect.y = stepY > 0 ? topSide : bottomSide;
            stepY = 0;
        }
    }
    return {static_cast<float>(rect.x - moving.x), static_cast<float>(rect.y - moving.y)};
}
Vector2 Collision::GetPushVector(const SDL_FRect& moving, const std::vector<SDL_Rect>& obstacles, Vector2 displacement) {
    SDL_FRect rect = { moving.x - displacement.x, moving.y - displacement.y, moving.w, moving.h };
    SDL_Rect collidedRect;
    const float moveStep = 4.0f;
    const float total = displacement.Length();
    const int steps = ceil(total / moveStep);
    float stepX = displacement.x / steps;
    float stepY = displacement.y / steps;
    for (int i = 0; i < steps; ++i) {
        rect.x += stepX;
        if (CheckAABB(rect, obstacles, collidedRect)) {
            int leftSide = collidedRect.x - rect.w;
            int rightSide = collidedRect.x + collidedRect.w;
            rect.x = stepX > 0 ? leftSide : rightSide;
            stepX = 0;
        }
        rect.y += stepY;
        if (CheckAABB(rect, obstacles, collidedRect)) {
            int topSide = collidedRect.y - rect.h;
            int bottomSide = collidedRect.y + collidedRect.h;
            rect.y = stepY > 0 ? topSide : bottomSide;
            stepY = 0;
        }
    }
    return { static_cast<float>(rect.x - moving.x), static_cast<float>(rect.y - moving.y) };
}
Vector2 Collision::GetPushVector(const SDL_Rect& moving, const std::vector<SDL_FRect>& obstacles, Vector2 displacement) {
    SDL_Rect rect = { moving.x - displacement.x, moving.y - displacement.y, moving.w, moving.h };
    SDL_FRect collidedRect;
    const float moveStep = 4.0f;
    const float total = displacement.Length();
    const int steps = ceil(total / moveStep);
    float stepX = displacement.x / steps;
    float stepY = displacement.y / steps;
    for (int i = 0; i < steps; ++i) {
        rect.x += static_cast<int>(stepX);
        if (CheckAABB(rect, obstacles, collidedRect)) {
            int leftSide = collidedRect.x - rect.w;
            int rightSide = collidedRect.x + collidedRect.w;
            rect.x = stepX > 0 ? leftSide : rightSide;
            stepX = 0;
        }
        rect.y += static_cast<int>(stepY);
        if (CheckAABB(rect, obstacles, collidedRect)) {
            int topSide = collidedRect.y - rect.h;
            int bottomSide = collidedRect.y + collidedRect.h;
            rect.y = stepY > 0 ? topSide : bottomSide;
            stepY = 0;
        }
    }
    return { static_cast<float>(rect.x - moving.x), static_cast<float>(rect.y - moving.y) };
}
Vector2 Collision::GetPushVector(const SDL_FRect& moving, const std::vector<SDL_FRect>& obstacles, Vector2 displacement) {
    SDL_FRect rect = { moving.x - displacement.x, moving.y - displacement.y, moving.w, moving.h };
    SDL_FRect collidedRect;
    const float moveStep = 4.0f;
    const float total = displacement.Length();
    const int steps = ceil(total / moveStep);
    float stepX = displacement.x / steps;
    float stepY = displacement.y / steps;
    for (int i = 0; i < steps; ++i) {
        rect.x += stepX;
        if (CheckAABB(rect, obstacles, collidedRect)) {
            int leftSide = collidedRect.x - rect.w;
            int rightSide = collidedRect.x + collidedRect.w;
            rect.x = stepX > 0 ? leftSide : rightSide;
            stepX = 0;
        }
        rect.y += stepY;
        if (CheckAABB(rect, obstacles, collidedRect)) {
            int topSide = collidedRect.y - rect.h;
            int bottomSide = collidedRect.y + collidedRect.h;
            rect.y = stepY > 0 ? topSide : bottomSide;
            stepY = 0;
        }
    }
    return { static_cast<float>(rect.x - moving.x), static_cast<float>(rect.y - moving.y) };
}
bool Collision::IsOnGround(const SDL_Rect& player, const SDL_Rect& tile) {
    float threshold = 1;
    return (player.y + player.h <= tile.y + threshold && player.y + player.h >= tile.y - threshold) &&
        (player.x + player.w > tile.x && player.x < tile.x + tile.w);
}
bool Collision::IsOnGround(const SDL_FRect& player, const SDL_Rect& tile) {
    float threshold = 1;
    return (player.y + player.h <= tile.y + threshold && player.y + player.h >= tile.y - threshold) &&
        (player.x + player.w > tile.x && player.x < tile.x + tile.w);
}
bool Collision::IsOnGround(const SDL_Rect& player, const SDL_FRect& tile) {
    float threshold = 1;
    return (player.y + player.h <= tile.y + threshold && player.y + player.h >= tile.y - threshold) &&
        (player.x + player.w > tile.x && player.x < tile.x + tile.w);
}
bool Collision::IsOnGround(const SDL_FRect& player, const SDL_FRect& tile) {
    float threshold = 1;
    return (player.y + player.h <= tile.y + threshold && player.y + player.h >= tile.y - threshold) &&
        (player.x + player.w > tile.x && player.x < tile.x + tile.w);
}
