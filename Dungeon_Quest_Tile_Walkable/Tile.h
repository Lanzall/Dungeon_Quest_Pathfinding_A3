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

	/*struct Node		//Defining the Node Structure within the grid system (HOW DO I DO THIS?)
	* I want to be able to define the node structure to represent each point on the grid
	* Should contain coordinates, cost variables, a way to track its parent node for path reconstruction
	{
		float gCost;
		float hCost;
		float fCost;
	};*/

	//Need to initialize two lists to manage nodes:
	//Open List - Storing nodes that are potential candiates to be explored. std:set or std:priority_queue are efficient for finding node with lowest fCost
	//Closed List - Stores nodes that have already been fully explored to prevent redundant processing

	//Perhaps Player broadcasts their coordinates at all times, the Tiles then can use that information to allocate the scores
	//The Minotaur will then use the scores provided by the tiles in order to pathfind towards the player
};

