#include "CEnemy.h"

CEnemy::CEnemy(bool canshoot)
{
	x = std::rand() % LEVEL_WIDTH;
	y = (std::rand() % 2) ? 0 : LEVEL_HEIGHT;
	healthPoints = 1;
	_canShoot = canshoot;
}

CEnemy::CEnemy(bool canshoot, double xSpawn, double ySpawn) : CEnemy()
{
	x = xSpawn;
	y = ySpawn;
	healthPoints = 2;
	_canShoot = canshoot;
}
 
CEnemy::CEnemy(bool canshoot, std::vector<coordinates> dstLst) : CEnemy()
{
	x = dstLst[0].first;
	y = dstLst[0].second;
	healthPoints = 999;
	_canShoot = canshoot;
	dstMngr = std::make_unique<destinationManager>(dstLst);
}


bool CEnemy::update()
{
	// if has planned route, move as planned
	//
	if ( hasPlannedRoute() )
	{
		xDest = dstMngr->move({ x,y }).first;
		yDest = dstMngr->move({ x,y }).second;
	}

	// otherwise we set enemy's destination (e.g. player's cords) in model update
	//
	calculateAngle(xDest, yDest);
	double toRadians = (angle*M_PI / 180);
	xVelocity = (velocity*cos(toRadians));	 // include math.h
	yVelocity = (velocity*sin(toRadians));	 // angle in radian, of course
	x += xVelocity;
	y += yVelocity;
	collider.x = static_cast<int>(x);
	collider.y = static_cast<int>(y);

	return true;
}

void CEnemy::onHit()
{
	//	If hp == 0 kill object
	if (!(--healthPoints))	
		isDead = true;

	//	Make entity aggresive if it's been hit
	/*if (hasPlannedRoute())
		setAggro();*/
}

void CEnemy::setAggro()
{
	if (hasPlannedRoute())
		dstMngr.reset();
}