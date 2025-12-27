#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "GameCharacter.h"
#include "Player.h"
#include "Minotaur.h"
#include "Tile.h"

const static std::string path_Hero = "Textures/Hero_sword.png";
const static std::string path_Boss = "Textures/Minotaur.png";
const static std::string path_Tiles[] = {
	"Textures/Tile_carpet_base.bmp", 
	"Textures/Tile_carpet_blood_1.bmp", 
	"Textures/Tile_carpet_bones.bmp"
};
enum Direction{North, East, South, West};
const static int RoomSize = 10;

class DungeonGame
{
public:
	DungeonGame(float tileSizeX, float tileSizeY);
	~DungeonGame();

	void Update(float DeltaTime);		// Delta time in seconds (e.g. 0.016f for 60fps)
	float TimeSinceLastPrint = 0.0f;		//The time since the last print to console, accumulates delta time
	const float PrintInterval = 1.0f;		//Interval of 1 second between prints to console

	void LoadTextures(SDL_Renderer* renderer);
	Player* Hero;
	Minotaur* Boss;
	void LoadRoom(const char* file);
	void MovePlayer(Direction dir);

	const char* StartingRoom = "Data/Rooms/Room01.bmp";
	Tile Tiles[RoomSize][RoomSize];
	SDL_Texture* CarpetTextures[2];
	
private:
	float tileSizeX;
	float tileSizeY;

};

