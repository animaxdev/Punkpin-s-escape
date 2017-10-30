#pragma once

#include "CWorldObject.h"
#include "defines.h"


//	Helper struct used to store destination
//	of enemy's consequent moves
//
struct destinationManager
{
	std::vector<coordinates> destinationList;
	unsigned int dstNr;

	destinationManager(std::vector<coordinates> dstLst)
	{
		destinationList = dstLst;
		dstNr = 0;
	}

	coordinates move(const coordinates cords)
	{
		//	couldnt't assign simple == , because doubles aren't
		//  accurate enough, so i made sections... 
		//
		if (	(cords.first / destinationList[dstNr].first) > 0.99
			&& (cords.second / destinationList[dstNr].second) > 0.99
			&& (cords.first / destinationList[dstNr].first) < 1.01
			&& (cords.second / destinationList[dstNr].second) < 1.01) {
			
				if (++dstNr == destinationList.size())
					dstNr = 0;
		}
		return destinationList[dstNr];
	}
};

//////////////////////////////////////////
//////////////////////////////////////////

class CEnemy : public CWorldObject
{
private:
	 
	double velocity;
	int healthPoints;
	std::unique_ptr<destinationManager> dstMngr;
	bool _canShoot;
	void setAggro();

public:

	CEnemy() : CWorldObject() {
		objectType = ENEMY;
		spawnTime = CTimer::getInstance().getTicks();
		velocity = 3.3;
	}

	CEnemy(bool canshoot);
	CEnemy(bool canshoot, double xSpawn, double ySpawn);
	CEnemy(bool canshoot, std::vector<coordinates> dstLst);

	~CEnemy() { }	 // todo

	bool update() override;
	void onHit();
	bool canShoot() const { return _canShoot; }
	bool hasPlannedRoute() const { return (dstMngr != NULL); }	
};