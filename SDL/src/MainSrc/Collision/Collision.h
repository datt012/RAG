#pragma once
#include <SDL_rect.h>
#include <vector>
#include "CMath.h"

class Collision
{
public:
	static bool CheckAABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool CheckAABB(const SDL_FRect& a, const SDL_Rect& b);
	static bool CheckAABB(const SDL_Rect& a, const SDL_FRect& b);
	static bool CheckAABB(const SDL_FRect& a, const SDL_FRect& b);

	static bool CheckAABB(const SDL_Rect& a, const std::vector<SDL_Rect>& others);
	static bool CheckAABB(const SDL_FRect& a, const std::vector<SDL_Rect>& others);
	static bool CheckAABB(const SDL_Rect& a, const std::vector<SDL_FRect>& others);
	static bool CheckAABB(const SDL_FRect& a, const std::vector<SDL_FRect>& others);

	static bool CheckAABB(const SDL_Rect& a, const std::vector<SDL_Rect>& others, SDL_Rect& collidedRect);
	static bool CheckAABB(const SDL_FRect& a, const std::vector<SDL_Rect>& others, SDL_Rect& collidedRect);
	static bool CheckAABB(const SDL_Rect& a, const std::vector<SDL_FRect>& others, SDL_FRect& collidedRect);
	static bool CheckAABB(const SDL_FRect& a, const std::vector<SDL_FRect>& others, SDL_FRect& collidedRect);

	static Vector2 GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle);

	static Vector2 GetPushVector(const SDL_Rect& moving, const SDL_Rect& obstacle, Vector2 displacement);

	static Vector2 GetPushVector(const SDL_Rect& moving, const std::vector<SDL_Rect>& obstacles, Vector2 displacement);
	static Vector2 GetPushVector(const SDL_FRect& moving, const std::vector<SDL_Rect>& obstacles, Vector2 displacement);
	static Vector2 GetPushVector(const SDL_Rect& moving, const std::vector<SDL_FRect>& obstacles, Vector2 displacement);
	static Vector2 GetPushVector(const SDL_FRect& moving, const std::vector<SDL_FRect>& obstacles, Vector2 displacement);

	static bool IsOnGround(const SDL_Rect& player, const SDL_Rect& tile);
	static bool IsOnGround(const SDL_FRect& player, const SDL_Rect& tile);
	static bool IsOnGround(const SDL_Rect& player, const SDL_FRect& tile);
	static bool IsOnGround(const SDL_FRect& player, const SDL_FRect& tile);
};