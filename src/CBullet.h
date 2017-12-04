#pragma once

#define _USE_MATH_DEFINES

#include "CWorldObject.h"
#include "math.h"



//


class CBullet final : public CWorldObject
{
private:

    bullet_data bulletData;
    std::string name;


public:
    CBullet(double xPos, double yPos);
    CBullet(double xPos, double yPos, double degreesAngle);
    CBullet(double xPos, double yPos, double xDst, double yDst);
    CBullet(double xPos, double yPos, double degreesAngle, int dmg, int rcl, int spd, typeObj type);

    ~CBullet();

    bool update() override;
    const bullet_data & getBulletData() const { return bulletData; }
};
