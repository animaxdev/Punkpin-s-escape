#include "CPlayer.h"

CPlayer::CPlayer()
{
	x = 320;
	y = 320;
	angle = 10.0;
	xVelocity = 0;
	yVelocity = 0;

	collider.w = TEXTURE_SIZE-8;
	collider.h = TEXTURE_SIZE-8;
	collider.x = static_cast<int>(x)+16;
	collider.y = static_cast<int>(y)+16;

	healthPoints = 0;
	isDead = true;
	scorePoints = 0;
}

bool CPlayer::move(std::string dir)
{
	if (dir == "UP" || dir == "up")
	{
		yVelocity = -velocity;
	}
	else if (dir == "DOWN" || dir == "down")
	{
		yVelocity = velocity;
	}
	else if (dir == "LEFT" || dir == "left")
	{
		xVelocity = -velocity;
	}
	else if (dir == "RIGHT" || dir == "right")
	{
		xVelocity = velocity;
	}
	else if (dir == "NONV" || dir == "nonv")
	{
		yVelocity = 0; //Moving = false;
	}
	else if (dir == "NONH" || dir == "nonh")
	{
		xVelocity = 0; //isMoving = false;
	}
	else
		std::cerr << "Wrong command for player's move() \n";

	return true;
}

bool CPlayer::update()
{
	x += xVelocity;
	collider.x = static_cast<int>(x);

	//	If too far to the left or right
	if ((x - 32 < 0) || (x + 32 > LEVEL_WIDTH))
	{
		//	Move back
		x -= xVelocity;
		collider.x = static_cast<int>(x);
	}

	y += yVelocity;// +delta;
	collider.y = static_cast<int>(y);

	//	If too far up or down
	if ((y - 32 < 0) || (y + 32 > LEVEL_HEIGHT))
	{
		//	Move back
		y -= yVelocity;
		collider.y = static_cast<int>(y);
	}
	return true;
}

void CPlayer::onHit()
{
	if (!(--healthPoints))
	{
		// Player's dead - todo
		//		- write score, etc.
		this->isDead = true;
	}
}

void CPlayer::addToScore(unsigned int points)
{
	scorePoints += points;
}

unsigned int CPlayer::getScore() const
{
	return scorePoints;
}