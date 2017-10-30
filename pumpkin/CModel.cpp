#include "CModel.h"


CModel::~CModel()
{
	std::cout << "gameModel destroyed\n";
}

void CModel::status()
{
	system("CLS");
	std::cout << "player at: ( " << playerObject->getX() << ", " 
			<< playerObject->getY() << " ) " << "    hp: " << playerObject->getHealthPoints() << "\n";

	for (auto &i : worldObject)
	{
		if (i->is(ENEMY))
		{
			std::cout << "enemy at: ( " << i->getX() << ", " << i->getY() << " ),    angle: "
				<< i->getAngle() << "*    spawn time: " << i->spawnTime/1000.f << " secs \n";
		}
	}
}

bool CModel::worldUpdate()
{
	//	Major player updater
	playerObject->update();

	//	Major game events updater
	gameEventManager->update(playerObject, worldObject);

	//	Map vs worldObjects (+ player) interactions
	mapObject->update(playerObject, worldObject);

	//	update worldObject vs worldObject (+ player) interactions:
	this->update();

	//	Deletions
	cleanUp();

	return true;
}

bool CModel::addBullet(typeObj type, int x, int y)
{
	switch (type)
	{
	case BULLET:

		worldObject.push_back(std::make_shared<CBullet>(	playerObject->getX(),
															playerObject->getY(),
															playerObject->getAngle() ) 
															);
		break;

	case ENEMY_BULLET:

		worldObject.push_back(std::make_shared<CBullet>(	x,
															y,
															playerObject->getX(),
															playerObject->getY() )
															);
		break;

	default:
		std::cout << "wrong object type while spawning bullet \n";
		break;
		return false;
	}

	return true;
}

bool CModel::addEnemy(bool canshoot)
{
	auto temp = std::make_shared<CEnemy>(canshoot);
	auto a = CTools::getInstance().readMapPopulation("data/dat/map_"+std::to_string(0) +".dat");
	
	for(auto &i : a)
	{ 
		// if there's only 1 coordinate - make enemy chase a player
		if (i.size() == 1)
		{ 
			auto tamp = std::make_shared<CEnemy>(0, i[0].first, i[0].second);
			worldObject.push_back(tamp);
		}
		else   // otherwise make enemy go through destination list
		{ 
			auto tamp = std::make_shared<CEnemy>(0, i);
			worldObject.push_back(tamp);
		}
	}

	return true;
}

bool CModel::cleanUp()
{
	//	Operations that change size of worldObject vector
	//	-	clean up
	//	-	time-based insertion events (enemy shoots after 3sec, etc.)
	//
	std::vector<CWorldObject>::size_type size = worldObject.size();
	for (std::vector<CWorldObject>::size_type i = 0; i < size; ++i)
	{
		//  Remove dead objects
		if (worldObject[i]->isDead)
		{
			worldObject.erase(worldObject.begin() + i);
			--size;
		}

		//	Well this is really ugly... need to rework
		//	Enemy shoots after desired period of time - (temporarily not used)
		else if (worldObject[i]->is(ENEMY) && std::static_pointer_cast<CEnemy>(worldObject[i])->canShoot()
			&& (CTimer::getInstance().timeElapsed(worldObject[i]->spawnTime, 3000)))
		{
			addBullet(ENEMY_BULLET, worldObject[i]->getX(), worldObject[i]->getY());
			++size;
		}
	}

	//	Remove dead events
	gameEventManager->cleanUp();
	return true;
}

bool CModel::killAll()
{
	this->worldObject.clear();
	return true;
}

bool CModel::update()
{
	for (auto &i : worldObject)
	{
		//	If object is an enemy
		if (i->is(ENEMY))
		{
			//	If enemy doesn't already have a planned route
			if (std::static_pointer_cast<CEnemy>(i)->hasPlannedRoute() == false)
			{
				//  Set destination of enemy as player's coordinates
				i->setDestinationCords(playerObject);
			}

			i->update();

			//	And check for collision with player
			if (i->checkCollision(playerObject))
			{
				//	if it's a ghost... yeah it's ugly statement, just for checking - todo
				if (std::static_pointer_cast<CEnemy>(i)->hasPlannedRoute())
					playerObject->isDead = true;
				else								//	if it's a zombie
				{
					i->isDead = true;				//	kill zombie
					playerObject->onHit();			//	hit player
				}
			}

			//	collision for any world object - not really needed, but let it be here
			/*for(auto &j : worldObject)
			{
				if (i != j && i->checkCollision(j))
				{
					i->moveBack();
				}
			}*/

		}

		//	if object is a bullet or a tile
		else if (i->is(BULLET))
		{
			i->update();

			//	Iterate through objects for collision detection
			for (auto &j : worldObject)
			{
				//	If collided with something
				if (j->is(ENEMY) && i->checkCollision(j))
				{
					//mapObject.reset(new CMap("maps/map_1.map"));
					i->isDead = true;					// Destroy the bullet
					std::static_pointer_cast<CEnemy>(j)->onHit();
					playerObject->addToScore(1);
					break;
				}
			}
		}

		//	if object is enemy's bullet
		else if (i->is(ENEMY_BULLET))
		{
			//  Set destination of bullet as player's coordinates
			i->setDestinationCords(playerObject);
			i->update();

			//  If enemy's bullet collided with player object
			if (i->checkCollision(playerObject))
			{
				playerObject->onHit();		//	Hit player
				i->isDead = true;			//	Destroy enemy's bullet
			}
		}
	}

	// if level finished, change map
	if (mapObject->isLevelFinished())
	{
		this->levelNr++;
		this->reset();
	}

	//	if player dead, reset current map
	if (playerObject->isDead)
		this->reset();

	return true;
}

bool CModel::reset()
{
	this->killAll();
	CTimer::getInstance().start();
	playerObject->setStartPos();
	playerObject->isDead = false;
	mapObject.reset(new CMap("data/map/map_" + std::to_string(this->levelNr) + ".map"));
	gameEventManager.reset(new CGameEventManager("data/dat/dat_" + std::to_string(this->levelNr) + ".dat"));
	return true;
}