#pragma once
#include "GameCharacter.h"
#include "Tile.h"
class Player :
    public GameCharacter
{
 public:
      void MoveUp();
      void MoveDown();
      void MoveLeft();
      void MoveRight();
      void Setlocation();
      void StartLocation();
      void PrintCoordinates();
	  Tile* HeroCurrentTile;      // Pointer to the tile the Player is currently on
};

