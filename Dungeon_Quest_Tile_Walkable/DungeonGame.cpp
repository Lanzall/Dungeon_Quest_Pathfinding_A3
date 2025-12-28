#include "DungeonGame.h"
#include <SDL3/SDL.h>
#include <vector>
#include <limits>
#include <algorithm>
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
	/*  Printing coords and other info once per second
	//Printing to console functions
	TimeSinceLastPrint += DeltaTime;	// Accumulate delta time

	//Check if 1 second or more has passed
	if (TimeSinceLastPrint >= PrintInterval)
	{
		//Print the player's coordinates to the console
		Hero->PrintCoordinates();
		//Print the Minotaur's coordinates to the console
		Boss->PrintBossCoordinates();
		//Print the tiles around the Boss with their costs
		PrintTilesAroundBoss();
		//Print the taxicab distance between the Hero and the Boss
		std::cout << "Taxicab Distance between Hero and Minotaur: " << TaxicabDistance(Hero->CoordinateX, Hero->CoordinateY, Boss->CoordinateX, Boss->CoordinateY) << std::endl;
		//Reset the timer
		TimeSinceLastPrint = 0.0f;
	}*/

	//Runs the A* pathfinding each frame (TEST)
	AStarPathfinding();

}

void DungeonGame::PrintTilesAroundBoss()
{
	// safety checks
	if (Boss == nullptr)
	{
		return;
	}

	// Ensure current tile references and heuristic costs are up-to-date
	GetCurrentTiles();
	SetHCosts();

	int bx = Boss->CoordinateX;
	int by = Boss->CoordinateY;

	auto printTileAt = [&](const char* name, int x, int y)
	{
		if (x >= 0 && x < RoomSize && y >= 0 && y < RoomSize)
		{
			Tile& tile = Tiles[x][y];
			std::cout << name << " (" << x << "," << y << ")"
				<< " g: " << tile.gCost
				<< " h: " << tile.hCost
				<< " f: " << tile.fCost
				<< std::endl;
		}
		else
		{
			std::cout << name << " out of bounds\n";
		}
	};

	printTileAt("Current", bx, by);
	printTileAt("North", bx, by - 1);
	printTileAt("East",  bx + 1, by);
	printTileAt("South", bx, by + 1);
	printTileAt("West",  bx - 1, by);

	//SetHCosts() is called before this function to ensure hCosts are current
	//Uses the bosses coordinates directly to index Tiles instead of relying on internal tile coordinates
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
	//Basic Safety
	if (Boss == nullptr || Hero == nullptr)
	{
		return;
	}

	//Ensure tiles and heuristic costs are up-to-date
	GetCurrentTiles();
	SetHCosts();

	if (BossCurrentTile == nullptr || HeroCurrentTile == nullptr)
	{
		return;
	}

	//Reset pathfinding state on all tiles
	for (int x = 0; x < RoomSize; ++x)
	{
		for (int y = 0; y < RoomSize; ++y)
		{
			Tile& t = Tiles[x][y];
			t.gCost = std::numeric_limits<float>::max();
			t.fCost = std::numeric_limits<float>::max();
			t.InOpenList = false;
			t.InClosedList = false;
			t.ParentTile = nullptr;
		}
	}

	Tile* startTile = BossCurrentTile;
	Tile* targetTile = HeroCurrentTile;

	startTile->gCost = 0;
	startTile->fCost = startTile->gCost + startTile->hCost;		//Set starting tile fCost
	std::list<Tile*> OpenTiles;		//Tiles to be evaluated
	std::list<Tile*> ClosedTiles;	//Tiles already evaluated
	OpenTiles.push_back(startTile);		//Add starting tile to open list
	startTile->InOpenList = true;	//Mark starting tile as in open list

	Tile* currentTile = nullptr;	//Pointer to the current tile being evaluated

	while (!OpenTiles.empty())
	{
		//Find node in open list with lowest fCost (tie-breaker on the hCost)
		currentTile = *std::min_element(OpenTiles.begin(), OpenTiles.end(), [](Tile* a, Tile* b)
			{
				if (a->fCost == b->fCost)
				{
					return a->hCost < b->hCost;	//Tie-breaker on hCost
				}
				return a->fCost < b->fCost;
			});

		//If target is reached, stop
		if (currentTile == targetTile)
		{
			break;		//Path found
		}

		//Remove current tile from open list and add to closed list
		OpenTiles.remove(currentTile);
		currentTile->InOpenList = false;
		currentTile->InClosedList = true;

		//Check neighbours
		Tile* neighbours[4] = { currentTile->NorthNeighbour, currentTile->EastNeighbour, currentTile->SouthNeighbour, currentTile->WestNeighbour };
		for (Tile* neighbour : neighbours)
		{
			if (neighbour == nullptr || !neighbour->Walkable || neighbour->InClosedList)	//Check if neighbour exists, is walkable, and not already evaluated
			{
				continue;	//Skip if null, not walkable, or already evaluated

				float tentativeGCost = currentTile->gCost + 1.0f; // uniform cost between adjacent tiles, tentative means it's a potential cost

				if (!neighbour->InOpenList)
				{
					//Discover a new node
					neighbour->ParentTile = currentTile;
					neighbour->gCost = tentativeGCost;
					neighbour->fCost = neighbour->gCost + neighbour->hCost;		//Update fCost with weighted heuristic
					OpenTiles.push_back(neighbour);
					neighbour->InOpenList = true;
				}
				else if (tentativeGCost < neighbour->gCost)
				{
					//This path to neighbour is better than previous one
					neighbour->ParentTile = currentTile;
					neighbour->gCost = tentativeGCost;
					neighbour->fCost = neighbour->gCost + neighbour->hCost;		//Update fCost with weighted heuristic
				}
			}
		}

		//After search completes, check if target was reached
		if (targetTile != startTile && targetTile->ParentTile == nullptr)
		{
			//No path found to target
			return;
		}

		//Reconstruct path from target to start
		std::vector<Tile*> path;	//Vector to store the path
		Tile* pathTile = targetTile;	//Start from target tile
		//Follow parent tiles back to start
		while (pathTile != nullptr)
		{
			path.push_back(pathTile);
			pathTile = pathTile->ParentTile;
		}
		std::reverse(path.begin(), path.end());	//Reverse the path to get it from start to target, now path[0] is start, last is target

		//Move the Minotaur one tile along the path if possible
		if (path.size() >= 2)
		{
			Tile* nextTile = path[1];	//Tile immediately after start tile
			//Update Minotaur's position to next tile
			Boss->CoordinateX = nextTile->TileTrackerX;
			Boss->CoordinateY = nextTile->TileTrackerY;
			//Update boss location rect (for rendering)
			Boss->SetLocation();
			//Update BossCurrentTile pointer
			GetCurrentTiles();
		}
	}
}

void DungeonGame::SetHCosts()
{
	//Hero's current coordinates are the target for the Minotaur
	int targetX = Hero->CoordinateX;
	int targetY = Hero->CoordinateY;

	//Iterate through all tiles in the room to set their hCosts
	for (int x = 0; x < RoomSize; x++)
	{
		for (int y = 0; y < RoomSize; y++)
		{
			Tile& tile = Tiles[x][y];
			//Calculate the hCost using taxicab distance
			tile.hCost = static_cast<float>(TaxicabDistance(x, y, targetX, targetY));
		}
	}
}

void DungeonGame::LowestFCostTile(Tile* Current)
{
	//This function will find the tile in the open list with the lowest fCost
	Tile* NearestTile[]{ Current->NorthNeighbour, Current->EastNeighbour, Current->SouthNeighbour, Current->WestNeighbour };		//Array of pointers to neighbouring tiles
	Tile* LowestTile = nullptr;		//Pointer to the tile with the lowest fCost found so far

	for (Tile* Neighbour : NearestTile)
	{
		if (Neighbour != nullptr && Neighbour->InOpenList)	//Check if the neighbour exists and is in the open list
		{
			if (LowestTile == nullptr || Neighbour->fCost < LowestTile->fCost)	//If no lowest tile yet or this neighbour has a lower fCost
			{
				LowestTile = Neighbour;	//Update the lowest tile
			}
		}
	}

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

	//After all tiles are created, wire up the neighbours and initialize pathfinding fields
	for (int x = 0; x < RoomSize; x++)
	{
		for (int y = 0; y < RoomSize; ++y)
		{
			Tile& t = Tiles[x][y];
			t.NorthNeighbour = (y > 0) ? &Tiles[x][y - 1] : nullptr;
			t.EastNeighbour = (x < RoomSize - 1) ? &Tiles[x + 1][y] : nullptr;
			t.SouthNeighbour = (y < RoomSize - 1) ? &Tiles[x][y + 1] : nullptr;
			t.WestNeighbour = (x > 0) ? &Tiles[x - 1][y] : nullptr;

			t.gCost = std::numeric_limits<float>::max();
			t.hCost = 0.0f;		//Will be set by SetHCosts()
			t.fCost = std::numeric_limits<float>::max();
			t.InOpenList = false;
			t.InClosedList = false;
			t.ParentTile = nullptr;
		}
	}

	//Set the heuristics now that the Hero location was set
	SetHCosts();

	//Update current tile pointers (the boss and player start locations were set earlier)
	GetCurrentTiles();
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

	SetHCosts();	//Update hCosts after player moves
}
