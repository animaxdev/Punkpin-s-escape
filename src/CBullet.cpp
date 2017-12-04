#include "CBullet.h"

CBullet::CBullet(double xPos, double yPos)
{
    x = xPos;
    y = yPos;
    collider.h = 4;
    collider.w = 16;
    bulletData.damage = 1;
    bulletData.recoil = 20;
    bulletData.speed = 2;
}

CBullet::CBullet(double xPos, double yPos, double degreesAngle) : CBullet(xPos, yPos)
{
    angle = degreesAngle;
	objectType = BULLET;
}

CBullet::CBullet(double xPos, double yPos, double xDst, double yDst) : CBullet(xPos, yPos)
{
	xDest = xDst; 
	yDest = yDst;
	angle = calculateAngle(xDest, yDest);
	objectType = ENEMY_BULLET;
}

CBullet::CBullet(double xPos, double yPos,
                 double degreesAngle, int dmg,
                 int rcl, int spd, typeObj type) : CBullet(xPos, yPos)
{
    angle = degreesAngle;
    objectType = type;
    bulletData.damage = dmg;
    bulletData.recoil = rcl;
    bulletData.speed = spd;
}

CBullet::~CBullet()
{
    //static int count = 0; std::cout<< ++count << " bullet ";
}

bool CBullet::update()
{
    double toRadians = (this->angle*M_PI / 180);
    x += bulletData.speed*cos(toRadians);  // include math.h
    y += bulletData.speed*sin(toRadians);  // angle in radian, of course
    collider.x = static_cast<int>(x);
    collider.y = static_cast<int>(y);

    if ((x + TEXTURE_SIZE < 0) || (x > LEVEL_WIDTH) ||
        (y + TEXTURE_SIZE < 0) || (y > LEVEL_HEIGHT) )
            isDead = true;

	return true;
}
