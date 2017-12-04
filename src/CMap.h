#pragma once

#include "CTools.h"
#include "CWorldObject.h"
#include "CTile.h"
#include <vector>

typedef std::vector< std::vector< std::shared_ptr< CTile >>> tileSet;

class CMap
{
private:
    //	a 2d vector of shared_ptrs
	tileSet mapTiles;

	int levelWidth;
	int levelHeight;
	bool _levelFinished;
	
	//	Write values to each tile
	bool setMap(std::string);

public:
    CMap(std::string file);
	~CMap() { std::cout << "MAP DESTROYED\n"; }

    const tileSet getMapTiles() { return mapTiles; }
    int getMapWidth() const { return levelWidth; }
    int getMapHeight() const { return levelHeight; }
    int isLevelFinished() const { return _levelFinished; }

	bool update(std::shared_ptr<CWorldObject> playerObject, 
        std::vector<std::shared_ptr<CWorldObject>> & worldObject);
};
