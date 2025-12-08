#pragma once
#include <SDL3_image/SDL_image.h>
#include "Pickup.h"
#include "Tile.h"
class GameCharacter
{
public:
	GameCharacter(); //constructor, called when created
	~GameCharacter();//destructor , called when destoroyed
	virtual void Attack(GameCharacter& other);
	virtual void Killed();
	virtual void Update(float dt); // for anything related to time, a mini update function
	virtual void collect(Pickup& pickup);
	virtual bool CanMoveBetweenRooms();
	bool GetIsAlive();
	bool TryMove();
	SDL_FRect* GetRect();
	SDL_FRect Rect;
	SDL_Texture* Texture;
	int CoordinateX;
	int CoordinateY;
	Tile* CharacterTile;
	Tile* NeighbourTile;
private:
	bool isAlive = true;
};

