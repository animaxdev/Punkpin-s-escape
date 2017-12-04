#pragma once

#include "CWorldObject.h"
#include "CMap.h"
#include "defines.h"
#include "CBullet.h"
#include "CWeapon.h"
#include "SDestinationManager.h"


//////////////////////////////////////////
//////////////////////////////////////////

class CEnemy final : public CWorldObject
{
private:

    static const int sightArea = 200;

    int healthPoints;
    std::unique_ptr<destinationManager> dstMngr;
    CWeapon * weapon;
    void setAggro();

    enemyIdData data;


public:

    CEnemy(int);

    CEnemy(enemyIdData dat, std::vector<coordinates> dstLst);

    ~CEnemy();

    bool update() override;
    void onHit();
    void onHit(std::shared_ptr<CBullet>);
    void handleDistance(double distance);

    bool hasPlannedRoute();
    bool canShoot() const;
    bool isAggressive() const { return data.aggro; }
    int getID() const { return data.id; }
    float getPercentHp() const { return ( (float)healthPoints/ (float)data.hp); }
    int getHp() const { return data.hp; }
    std::string getName() const { return data.name; }

    double getAngleToRender() const;

    std::vector<std::shared_ptr<CWorldObject>> shoot();
    std::vector<std::shared_ptr<CWorldObject>> shoot(const CWorldObject & obj);
};



