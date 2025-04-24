#pragma once
#include <SDL_rect.h>
#include <vector>

class Collision
{
public:
	static bool CheckAABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool CheckAABB(const SDL_Rect& a, const std::vector<SDL_Rect>& others);
	static SDL_Point GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle);
	static bool IsOnGround(const SDL_Rect& player, const SDL_Rect& tile);
};