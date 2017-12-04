#include "CMap.h"


CMap::CMap(std::string file)
{
    setMap(file);
    _levelFinished = false;
}


bool CMap::setMap(std::string file)
{
	//	Resize vectors to coresponding size-values
	std::vector<std::vector<char>> map = CTools::getInstance().readMap(file);

	int mapSizeX = static_cast<int>(map[0].size());			//	X
	int mapSizeY = static_cast<int>(map.size());			//	Y

	mapTiles.resize(mapSizeX);
	for (int i = 0; i < mapSizeX; i++)
		mapTiles[i].resize(mapSizeY);

	//	Set level dimensions
	this->levelWidth = mapSizeX * CTile::tileSize;
	this->levelHeight = mapSizeY * CTile::tileSize;

	//	Fill vectors with CTile objects
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			this->mapTiles[i][j] = std::make_shared<CTile>(	i*CTile::tileSize,
														j*CTile::tileSize,
						(map[j][i] == 'g') ? GROUND : ( map[j][i] == 'w' ? WALL : DOOR) );
		}
	}

	return true;
}

bool CMap::update(std::shared_ptr<CWorldObject> playerObject, std::vector<std::shared_ptr<CWorldObject>> &worldObject)
{
	for (auto &i : getMapTiles())
	{
		for (auto &j : i)
		{
			if (j->getTileType() == WALL)
			{
                j->update();
				//	If collided with player - move him back
                if (j->checkCollision(playerObject))
                    playerObject->moveBack();

				//	Now check collision for any world object
				for (auto &k : worldObject)
				{
					//	If tile collided with enemy
                    if (k->is(ENEMY) && k->checkCollision(j))
                    {
                        k->moveBack();
                    }
                        //k->moveBack();		//	move him back

                    //	If bullet encountered wall
                    else if ((k->is(BULLET) || k->is(ENEMY_BULLET)) && k->checkCollision(j))
                    {
                        k->isDead = true;	//	kill the bullet
                        j->onHit();
                    }
				}
			}
			if (j->getTileType() == DOOR)
			{
				if (j->checkCollision(playerObject))
				{
					this->_levelFinished = true;
				}
			}
		}
	}
	return true;
}
