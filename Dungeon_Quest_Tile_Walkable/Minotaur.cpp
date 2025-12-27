#include "Minotaur.h"
#include <iostream>

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

void Minotaur::PrintBossCoordinates()
{
	std::cout << "Minotaur Coordinates are: \nX: " << CoordinateX << "\nY: " << CoordinateY << std::endl;		// Prints the Minotaur's coordinates to the terminal using the CoordinateX and CoordinateY variables
}
