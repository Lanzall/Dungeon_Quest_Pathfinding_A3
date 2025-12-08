#pragma once
enum Direction { North, East, South, West, Unknown = -1 };

enum MoveResultAction
{
	Undefined = -1,
	Blocked = 0,
	MoveOK = 1,
	Fight = 2,
	LoadNewRoom = 3 // if tile = nullptr, load new room
};

