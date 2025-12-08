#include "DungeonGame.h"
#include <SDL3/SDL.h>
DungeonGame::DungeonGame(float tileSizeX, float tileSizeY)
{
	this->tileSizeX = tileSizeX;
	this->tileSizeY = tileSizeY;
}

DungeonGame::~DungeonGame()
{
	delete this->Hero;
}

void DungeonGame::Update(float DeltaTime)
{
}

void DungeonGame::LoadTextures(SDL_Renderer* renderer)
{
	this->Hero = new Player;
	//Load all textures
	this->Hero->Texture = IMG_LoadTexture(renderer, path_Hero.c_str());
	this->Hero->Rect.w = tileSizeX;
	this->Hero->Rect.h = tileSizeY;
	for (int i = 0; i <2; i++) 
	{
		this->CarpetTextures[i] = IMG_LoadTexture(renderer, path_Tiles[i].c_str()); //there is some kind of problem here that needs to be fixed
		SDL_ScaleMode scaleMode = SDL_SCALEMODE_NEAREST;
		SDL_GetTextureScaleMode(this->CarpetTextures[i], &scaleMode);
	}
}

void DungeonGame::LoadRoom(const char* file)// parse the BMP file into here - successful
{
	SDL_Surface* surface = SDL_LoadBMP(file);
	const SDL_PixelFormatDetails* pixelDetails = SDL_GetPixelFormatDetails(surface->format);
	const Uint8 bpp = SDL_BYTESPERPIXEL(surface->format);
	SDL_Color col;
	SDL_Color Wall = { 0,0,0,255 }; // Unwalkable tile
	this->Hero->StartLocation();

	for (int y = 0; y <RoomSize && y < surface->h; y++) 
	{
		for (int x = 0; x <RoomSize && x < surface->w; x++)
		{
			Uint8* pixel = static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp; //Get color of pixels
			SDL_GetRGB(*reinterpret_cast<Uint32*>(pixel), pixelDetails, NULL, &col.r, &col.g, &col.b); // gets RGB value of pixels to be interpreted by game
			// parse the color data into the tiles
			this->Tiles[x][y].CompareColors(col,Wall); //Managed to fetch color, Just need fix up col2
			//convert the string into a texture file
			this->Tiles[x][y].Configure(col, x, y, tileSizeX, this->CarpetTextures);
			this->Tiles[x][y].SetCoordinate(x, y);
			//this->Tiles[x][y].
			//Get the neighbouring tiles here
		};
	}
}

void DungeonGame::MovePlayer(Direction dir)
{
	if (dir == Direction::North && Tiles[Hero->CoordinateX][Hero->CoordinateY - 1].Walkable)
		Hero->MoveUp();
	if (dir == Direction::East && Tiles[Hero->CoordinateX + 1][Hero->CoordinateY].Walkable)
		Hero->MoveRight();
	if (dir == Direction::South && Tiles[Hero->CoordinateX][Hero->CoordinateY + 1].Walkable)
		Hero->MoveDown();
	if (dir == Direction::West && Tiles[Hero->CoordinateX - 1][Hero->CoordinateY].Walkable)
		Hero->MoveLeft();
}
