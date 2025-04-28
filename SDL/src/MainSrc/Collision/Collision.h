#pragma once
#include <SDL_rect.h>
#include <vector>
#include "CMath.h"

class Collision
{
public:
	static bool CheckAABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool CheckAABB(const SDL_Rect& a, const std::vector<SDL_Rect>& others);
	static Vector2 GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle);
	static Vector2 GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle, const Vector2& velocity);
	static Vector2 GetPushVector(const SDL_Rect& moving, const std::vector<SDL_Rect>& obstacles, const Vector2& velocity);
	static bool IsOnGround(const SDL_Rect& player, const SDL_Rect& tile);
};