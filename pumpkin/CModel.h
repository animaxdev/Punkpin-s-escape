#pragma once

#include "CWorldObject.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CBullet.h"
#include "CEnemy.h"
#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <algorithm>
#include "CGameEventManager.h"


class CModel final : public std::enable_shared_from_this<CModel>
{

private:
	//	Vector of shared pointers to CWorldObject
	typedef std::vector<std::shared_ptr<CWorldObject>> world_objects;
	world_objects worldObject;
	std::shared_ptr<CPlayer> playerObject = std::make_shared<CPlayer>();
	std::shared_ptr<CMap> mapObject = std::make_shared<CMap>("data/map/map_0.map");
	std::unique_ptr<CGameEventManager> gameEventManager = std::make_unique<CGameEventManager>("data/dat/dat_0.dat");
	int levelNr;

	bool cleanUp();
	bool killAll();
	bool update();
	

public:
	CModel() { 
		levelNr = 0; 
	}
	~CModel();

	void status();
	bool worldUpdate();
	bool addBullet(typeObj, int x = 0, int y = 0);
	bool addEnemy(bool canshoot = 0);
	
	const world_objects getWorldObjects() const { return worldObject; }
	std::shared_ptr<CPlayer> getPlayerObject() const {	return playerObject;	}
	std::shared_ptr<CMap> getMapObject() const { return mapObject; }
	std::shared_ptr<CModel> getModel() { return shared_from_this(); }
	const int getLevelNr() { return levelNr; }
	bool reset();

};