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
	Tile* NorthNeighbour;
	Tile* EastNeighbour;
	Tile* SouthNeighbour;
	Tile* WestNeighbour;

	//Using floats instead of ints for the Pathfinding costs to allow weighting the heuristic at 1.5 as Minecraft does for faster pathfinding if obstacles are present
	float gCost;
	float hCost;
	float fCost;

	bool InOpenList;
	bool InClosedList;

	float PredictedFCost();	//Used to store predicted fCost for nodes not yet in open list

	Tile* ParentTile;	//Pointer to the parent tile for path reconstruction
};

