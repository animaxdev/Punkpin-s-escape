#pragma once

#include "CWorldObject.h"
#define _USE_MATH_DEFINES
#include "math.h"

class CBullet : public CWorldObject
{
private:
	static const int bulletSpeed = 20;

public:
	CBullet(double xPos, double yPos, double degreesAngle);
	CBullet(double xPos, double yPos, double xDst, double yDst);
	~CBullet();

	bool update() override;
};