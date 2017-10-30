#include "CGameEventManager.h"

CGameEventManager::CGameEventManager(std::string path)
{
	pathToFile = path; 
	this->setEventManager();
}

CGameEventManager::~CGameEventManager()
{
	std::cout << "Game Event Manager DESTROYED\n";
}

bool CGameEventManager::setEventManager()
{
	auto temp = CTools::getInstance().readMapPopulation_(this->pathToFile);

	double tempX, tempY;

	for (auto &i : temp)
	{
		tempX = i[0][0].first;
		tempY = i[0][0].second;
		i.erase(i.begin());

		gameEvents.push_back(std::make_shared<CGameEvent>(tempX, tempY, 2, i));
	}

	return true;
}

bool CGameEventManager::cleanUp()
{
	std::vector<CWorldObject>::size_type size = gameEvents.size();
	for (std::vector<CWorldObject>::size_type i = 0; i < size; ++i)
	{
		//  Remove dead objects
		if (gameEvents[i]->isDead)
		{
			gameEvents.erase(gameEvents.begin() + i);
			--size;
		}
	}
	return true;
}

bool CGameEventManager::update(const std::shared_ptr<CPlayer> playerObject, std::vector<std::shared_ptr<CWorldObject>> &worldObject)
{
	for (auto &i : gameEvents)
	{
		// if player collided with event
		if (i->checkCollision(playerObject))
		{
			// spawn enemies
			for (auto &j : i->getEnemyPositions())
			{
				if (j.size() == 1)
				{
					auto tamp = std::make_shared<CEnemy>(0, j[0].first, j[0].second);
					worldObject.push_back(tamp);
				}
				else
				{
					auto tamp = std::make_shared<CEnemy>(0, j);
					worldObject.push_back(tamp);
				}
			}
			i->isDead = true;
		}
	}
	return true;
}
