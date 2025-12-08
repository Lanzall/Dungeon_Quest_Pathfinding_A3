#include "Tile.h"
#include <SDL3/SDL.h>
#include <iostream>
bool Tile::CompareColors(SDL_Color& col1, SDL_Color& col2) //parse BMP file into col1 and set col2 to black/white when BMP is equal to black, set walkable to false
{
	return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b;
}

void Tile::Configure(SDL_Color& color, float x, float y, float size, SDL_Texture* textures[])
{
	SDL_Color Wall = { 0,0,0,255 }; // color for unwalkable;

	if (CompareColors(color, Wall))
	{
		Walkable = false;
	}
	else
	{
		Walkable = true;
	}


	if (Walkable == true)
	{
		this->Texture = textures[0];
	}
	

	if (Walkable == false)
	{
		this->Texture = nullptr;
		//std::cout << "Tile configured at position (" << x << "," << y << ") <<std::endl"; (re-use this if tiles ever need to be tested again)
	}
	this->Rect.w = size;
	this->Rect.h = size;
	this->Rect.x = x * size;
	this->Rect.y = y * size;

	///std::cout << "Tile configured at position (" << x << "," << y << ") <<std::endl";
	
}

void Tile::SetCoordinate(float Tilex, float Tiley)
{
	//std::cout << "tile at position (" << Tilex << "," << Tiley << ") <<std::endl";
	this->TileTrackerX;
	this->TileTrackerY = Tiley;
	this->TileTrackerX = Tilex;
}

void Tile::GetNeighbour(float current, float dir)
{
}
