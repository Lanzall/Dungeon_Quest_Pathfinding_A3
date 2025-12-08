#pragma once
#include "Tile.h"
#include "GameCharacter.h"
#include "Enums.h"
class MoveResult
{
public:
	MoveResult();
	MoveResult(Tile* n);
	~MoveResult();
	Tile* GetTile();

	void setAction(MoveResultAction a);
	MoveResultAction GetAction();
private:
	GameCharacter* character;
	MoveResultAction action;
	Tile* tile;
};

