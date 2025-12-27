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
};

