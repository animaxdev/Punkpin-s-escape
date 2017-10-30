#include "CBullet.h"

CBullet::CBullet(double xPos, double yPos, double degreesAngle)
{
	x = xPos;
	y = yPos;
	angle = degreesAngle;
	collider.h = 32; collider.w = 16;
	objectType = BULLET;
}

CBullet::CBullet(double xPos, double yPos, double xDst, double yDst)
{
	x = xPos;
	y = yPos;
	collider.h = 32; collider.w = 16;
	xDest = xDst; 
	yDest = yDst;
	angle = calculateAngle(xDest, yDest);
	objectType = ENEMY_BULLET;
}

CBullet::~CBullet()
{
	//static int count = 0; std::cout<< "bullets so far: " << ++count << std::endl;
}

bool CBullet::update()
{
	double toRadians = (angle*M_PI / 180);
	x += bulletSpeed*cos(toRadians);  // include math.h
	y += bulletSpeed*sin(toRadians);  // angle in radian, of course
	collider.x = static_cast<int>(x+24);
	collider.y = static_cast<int>(y);

	if ((x + TEXTURE_SIZE < 0) || (x > LEVEL_WIDTH) ||
		(y + TEXTURE_SIZE < 0) || (y > LEVEL_HEIGHT) )
			isDead = true;

	return true;
}
