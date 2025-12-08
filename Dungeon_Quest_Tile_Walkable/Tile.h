#pragma once
#include <SDL3/SDl.h>
class Tile
{
public:
	static bool CompareColors(SDL_Color& col1, SDL_Color& col2);
	bool Walkable;
	SDL_Texture* Texture;
	SDL_FRect Rect;
	void Configure(SDL_Color& color, float x, float y, float size, SDL_Texture* textures[]);
	int TileTrackerX;
	int TileTrackerY;
	void SetCoordinate(float Tilex, float Tiley);
	void GetNeighbour(float current, float dir);
};

