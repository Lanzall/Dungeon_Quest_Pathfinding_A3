#include "Minotaur.h"

void Minotaur::SetLocation()
{
	Rect.y = CoordinateY * 100;
	Rect.x = CoordinateX * 100;
}

void Minotaur::StartLocation()
{
	CoordinateX = 1;
	CoordinateY = 8;
}
