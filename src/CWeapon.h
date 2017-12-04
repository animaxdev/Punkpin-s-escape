#pragma once

#include "CBullet.h"
#include "defines.h"
#include "CTools.h"

//  CWeapon class:
//  - spawns bullets
//  - manages bullet's damage, handles reloads, etc.
//


class CWeapon
{
private:
    //const static int spreadFactor = 5;
    Uint32 currTime, rt;
    weaponIdData weaponData;
    int bulletCount;
    float reloadProgress;
    void reload();

public:

    CWeapon();
    CWeapon(weaponIdData);
    ~CWeapon() {}

    bool update();

    std::vector<std::shared_ptr<CWorldObject>> spawnBullet( int xPos, int yPos, double deg, typeObj type );

    const float getReloadProgress() const { return reloadProgress; }
    int getBulletCount() const { return bulletCount; }
    int getBulletCountMax() const { return weaponData.bulletCountMax; }
    const std::string getWeaponName() const { return weaponData.name; }
    int getWeaponDamage() const { return weaponData.bulletData.damage; }
};
