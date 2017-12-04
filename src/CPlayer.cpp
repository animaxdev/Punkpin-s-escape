#include "CPlayer.h"

CPlayer::CPlayer()
{
    x = 320;
    y = 320;
    angle = 10.0;
    xVelocity = 0;
    yVelocity = 0;

    healthPoints = 0;
    isDead = true;
    scorePoints = 0;
}

CPlayer::~CPlayer()
{
    std::cout << "Player object destroyed \n";
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

    //  If too far to the left or right
    if ((x - TEXTURE_SIZE/2 < 0) || (x + TEXTURE_SIZE/2 > LEVEL_WIDTH))
    {
        //  Move back
        x -= xVelocity;
        collider.x = static_cast<int>(x);
    }

    y += yVelocity;

    //  If too far up or down
    if ((y - TEXTURE_SIZE/2 < 0) || (y + TEXTURE_SIZE/2 > LEVEL_HEIGHT))
    {
        //  Move back
        y -= yVelocity;
    }

    // center hitbox
    collider.x = static_cast<int>(x)+TEXTURE_SIZE/(2*HITBOX_RATIO);
    collider.y = static_cast<int>(y)+TEXTURE_SIZE/(2*HITBOX_RATIO);

    if( this->hasWeapon() ) {
        this->weapons[activeWeapon].update();
    }

    // set flag
    wasHit = false;

    return true;
}

void CPlayer::onHit()
{
    wasHit = true;
    if (!(--healthPoints))
    {
        // Player's dead - todo
        //      - write score, etc.
        this->isDead = true;
    }
}

void CPlayer::addToScore(unsigned int points)
{
    scorePoints += points;
}

int CPlayer::getScore() const
{
    return scorePoints;
}

std::vector<std::shared_ptr<CWorldObject>> CPlayer::shoot()
{
    return weapons[this->activeWeapon].spawnBullet(x,y,this->getAngle(),BULLET);
}

void CPlayer::addWeapon(CWeapon weapon)
{
    weapons.push_back(weapon);
}

void CPlayer::switchWeapon()
{
    if(++(this->activeWeapon) >= this->weapons.size())
        this->activeWeapon = 0;
}

const CWeapon * CPlayer::getActiveWeapon()
{
    if(weapons.empty() == false)
        return &weapons[this->activeWeapon];
    else
        return nullptr;
}

bool CPlayer::hasWeapon()
{
    return (weapons.empty() == false);
}
