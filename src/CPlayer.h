#pragma once

#include "CWorldObject.h"
#include "CWeapon.h"




class CPlayer final : public CWorldObject
{
private:

    static const int maxHp = 10;
    int velocity = 4;
    int activeWeapon = 0;
    int healthPoints;
    int scorePoints;

    // inventory
    std::vector<CWeapon> weapons;



public:

    CPlayer();
    ~CPlayer();

    bool move(std::string);
    bool update() override;
    void onHit();

    void addToScore(unsigned int);
    int getScore() const;
    int getHealthPoints() const { return healthPoints; }
    void setVelocity(int val) { velocity = val; }
    float getMaxHp() const { return static_cast<float>(maxHp);}
    void setStartPos() { x = 100, y = 100; healthPoints = 6; weapons.clear(); }

    std::vector<std::shared_ptr<CWorldObject>> shoot();

    void addWeapon(CWeapon weapon);
    bool hasWeapon();
    void switchWeapon();
    const CWeapon * getActiveWeapon();
};
