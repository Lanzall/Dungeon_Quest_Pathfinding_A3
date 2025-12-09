#include "Player.h"
#include <iostream>

void Player::MoveUp()
{
	CoordinateY -= 1;
}

void Player::MoveDown()
{
	CoordinateY += 1;
}

void Player::MoveLeft()
{
	CoordinateX -= 1;
}

void Player::MoveRight()
{
	CoordinateX += 1;
}

void Player::Setlocation()
{
	Rect.y = CoordinateY * 100;
	Rect.x = CoordinateX * 100;
}

void Player::StartLocation()
{
	CoordinateX = 8;
	CoordinateY = 1;
}