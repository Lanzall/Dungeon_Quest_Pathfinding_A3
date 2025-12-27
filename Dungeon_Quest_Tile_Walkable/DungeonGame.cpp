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
	delete this->Boss;		//called when this function closes, prevents memory leaks
}

void DungeonGame::Update(float DeltaTime)
{
	//Printing to console functions
	TimeSinceLastPrint += DeltaTime;	// Accumulate delta time

	//Check if 1 second or more has passed
	if (TimeSinceLastPrint >= PrintInterval)
	{
		//Print the player's coordinates to the console
		Hero->PrintCoordinates();
		//Print the Minotaur's coordinates to the console
		Boss->PrintBossCoordinates();
		//Print the taxicab distance between the Hero and the Boss
		std::cout << "Taxicab Distance between Hero and Minotaur: " << TaxicabDistance(Hero->CoordinateX, Hero->CoordinateY, Boss->CoordinateX, Boss->CoordinateY) << std::endl;
		//Reset the timer
		TimeSinceLastPrint = 0.0f;
	}
}

void DungeonGame::GetCurrentTiles()		//Grabs the current tiles that the Hero and Boss are on
{
	HeroCurrentTile = &Tiles[Hero->CoordinateX][Hero->CoordinateY];
	BossCurrentTile = &Tiles[Boss->CoordinateX][Boss->CoordinateY];
}

int DungeonGame::TaxicabDistance(int x1, int y1, int x2, int y2)		//Calculates the taxicab distance between the two points, not including diagonals
{
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

void DungeonGame::AStarPathfinding()
{
	std::list<Tile*> OpenTiles;		//Tiles to be evaluated
	std::list<Tile*> ClosedTiles;	//Tiles already evaluated

	StartTile = BossCurrentTile;	//Starting tile is where the Boss currently is
	StartTile->fCost = 0;	//Starting tile has no cost to reach itself
	OpenTiles.push_front(StartTile);	//Add starting tile to open list
}

void DungeonGame::LoadTextures(SDL_Renderer* renderer)
{
	this->Hero = new Player;
	//Load all textures
	this->Hero->Texture = IMG_LoadTexture(renderer, path_Hero.c_str());
	SDL_SetTextureScaleMode(this->Hero->Texture, SDL_SCALEMODE_NEAREST);		//Point filtering like Blender, makes the pixels appear sharp as intended
	this->Hero->Rect.w = tileSizeX;
	this->Hero->Rect.h = tileSizeY;

	this->Boss = new Minotaur;
	//Loading the Minotaur texture (It just has it on hand, knows it exists)
	this->Boss->Texture = IMG_LoadTexture(renderer, path_Boss.c_str());		//c string converts string path to be able to load it
	SDL_SetTextureScaleMode(this->Boss->Texture, SDL_SCALEMODE_NEAREST);	//Boss Sprite scalemode is set to nearest, makes the sprite appear crisp and not blurred
	this->Boss->Rect.w = tileSizeX;
	this->Boss->Rect.h = tileSizeY;

	for (int i = 0; i <2; i++) 
	{
		this->CarpetTextures[i] = IMG_LoadTexture(renderer, path_Tiles[i].c_str()); //there is some kind of problem here that needs to be fixed
		SDL_ScaleMode scaleMode = SDL_SCALEMODE_NEAREST;
		SDL_GetTextureScaleMode(this->CarpetTextures[i], &scaleMode);
		SDL_SetTextureScaleMode(this->CarpetTextures[i], SDL_SCALEMODE_NEAREST);		//Realised that the floor and the hero weren't actually set to scalemode_nearest, fixed this
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
	this->Boss->StartLocation();

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
