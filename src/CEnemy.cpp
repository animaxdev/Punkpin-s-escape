#include "CEnemy.h"
CEnemy::CEnemy(int sz) : CWorldObject()
{
    objectType = ENEMY;
    spawnTime = CTimer::getInstance().getTicks();
    size = sz;
}

CEnemy::CEnemy(enemyIdData dat, std::vector<coordinates> dstLst) : CEnemy(dat.size)
{
    x = dstLst[0].first;
    y = dstLst[0].second;
    data = dat;
    dstMngr = std::make_unique<destinationManager>(dstLst);
    healthPoints = data.hp;

    if(data.weapon_id < 1)
        weapon = nullptr;
    else
        weapon = new CWeapon(CTools::getInstance().GetDataByID(data.weapon_id, CTools::getInstance().getWeaponData()));
}

CEnemy::~CEnemy()
{
    delete weapon;
}


bool CEnemy::update()
{
	// if has planned route, move as planned
	//
	if ( hasPlannedRoute() )
	{
        auto dir =  dstMngr->move({ x,y }, this->data.velocity);
        xDest = dir.first;
        yDest = dir.second;
	}

    this->angle = calculateAngle(xDest, yDest);

    this->move( this->angle, this->data.velocity );

    //  center hitbox
    collider.x = static_cast<int>(x)+TEXTURE_SIZE/(2*HITBOX_RATIO);
    collider.y = static_cast<int>(y)+TEXTURE_SIZE/(2*HITBOX_RATIO);

    if(weapon != nullptr)
        weapon->update();

    wasHit = false;

	return true;
}

void CEnemy::onHit()
{
	//	If hp == 0 kill object
	if (!(--healthPoints))	
		isDead = true;

	//	Make entity aggresive if it's been hit
    setAggro();
}

void CEnemy::setAggro()
{
    if(data.can_aggro)
    {
        //  set temporary aggro
        if(dstMngr != nullptr)
        {
            dstMngr->active = false;
            data.aggro = true;
        }

        //  set permanent aggro
        if(this->isHit() == true)
        {
            dstMngr.reset();
        }
    }
}

void CEnemy::onHit(std::shared_ptr<CBullet> bullet)
{
    //  Move object back if it can move
    if(this->data.velocity != 0)
        this->move(bullet->getAngle(), bullet->getBulletData().recoil);

    this->healthPoints -= bullet->getBulletData().damage;

    //	If hp == 0 kill object
    if (!(this->healthPoints))
        this->isDead = true;

    //	Make entity aggresive if it's been hit
    wasHit = true;
    setAggro();

}

bool CEnemy::hasPlannedRoute()
{
    if(dstMngr == nullptr)
        return false;
    else if(dstMngr->destinationList.size() < 2)
        return false;
    else if(dstMngr->active == false)
        return false;
    else
        return true;
}

void CEnemy::handleDistance(double distance)
{
    if(data.aggro_radius > 0 && distance < data.aggro_radius)
        setAggro();
    else
    {
        if(dstMngr != nullptr && dstMngr->destinationList.size() > 1)
        {
            data.aggro = false;
            dstMngr->active = true;
        }
    }
}

bool CEnemy::canShoot() const {
    if(weapon != nullptr)
        return true;
    else
        return false;
}

double CEnemy::getAngleToRender() const {
    if(this->data.static_angle == -1)
        return (this->angle);
    else
        return (this->data.static_angle);
}

std::vector<std::shared_ptr<CWorldObject>> CEnemy::shoot()
{
    return weapon->spawnBullet(x + (data.size*TEXTURE_SIZE)/4,
                               y + (data.size*TEXTURE_SIZE)/4,
                               this->getAngle(),
                               ENEMY_BULLET);
}

std::vector<std::shared_ptr<CWorldObject>> CEnemy::shoot(const CWorldObject & obj)
{
    return weapon->spawnBullet(x,y,obj.getAngle(), ENEMY_BULLET);
}
