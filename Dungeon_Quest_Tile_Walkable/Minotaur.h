#pragma once
#include "GameCharacter.h"
#include "Tile.h"

class Minotaur :
    public GameCharacter

{
public:
    void SetLocation();
    void StartLocation();
	void PrintBossCoordinates();
	Tile* CurrentTile;      // Pointer to the tile the Minotaur is currently on
};

