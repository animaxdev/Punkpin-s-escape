#include "CWeapon.h"


CWeapon::CWeapon() {
    bulletCount = 0;

    weaponData.reloadTime = 1;
    weaponData.bulletCountMax = 6;
    weaponData.shootsAtOnce = 72;
    weaponData.bulletData.recoil = 20;
    weaponData.bulletData.damage = 1;
    currTime = CTimer::getInstance().getTicks();
}

CWeapon::CWeapon(weaponIdData data)
{
    bulletCount = 1;
    weaponData = data;
}

bool CWeapon::update()
{
    rt = CTimer::getInstance().getTicks();

    if(bulletCount < weaponData.bulletCountMax)
       this->reload();
    else
       this->currTime = rt;

    return true;
}

std::vector<std::shared_ptr<CWorldObject>> CWeapon::spawnBullet( int xPos, int yPos, double deg, typeObj type )
{
    std::vector<std::shared_ptr<CWorldObject>> bulletsToPush;

    if(this->bulletCount > 0)
    {
        --(this->bulletCount);
        this->currTime = rt;

        for(int i = 0 ; i<weaponData.shootsAtOnce ; i++)
        {
            int dir = ((i%2 == 1) ? i : -i);
            deg = deg + (dir * weaponData.spreadFactor);

            auto temp = std::make_shared<CBullet>(xPos, yPos, deg,
                                             weaponData.bulletData.damage,
                                             weaponData.bulletData.recoil,
                                             weaponData.bulletData.speed,
                                             type);

            bulletsToPush.push_back(temp);
        }

        return bulletsToPush;

    }
    else
    {
        return {};
    }
}

void CWeapon::reload()
{
    this->reloadProgress = (rt - this->currTime) / static_cast<float>(weaponData.reloadTime);

    if( reloadProgress >= 1.0 )
    {
        this->currTime = rt;
        if(bulletCount < weaponData.bulletCountMax)
             ++bulletCount;
    }
}


