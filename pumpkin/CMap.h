#pragma once

#include "CTools.h"
#include "CWorldObject.h"
#include "CTile.h"
#include <vector>

typedef std::vector< std::vector< std::shared_ptr< CTile >>> tileSet;

class CMap
{
private:
	//	Map is a vector of vectors of shared_ptrs
	tileSet mapTiles;

	int levelWidth;
	int levelHeight;
	bool _levelFinished;
	
	//	Write values to each tile
	bool setMap(std::string);

public:
	CMap(std::string file) { 
		setMap(file);
		_levelFinished = false;
	}

	~CMap() { std::cout << "MAP DESTROYED\n"; }

	const tileSet getMapTiles() const { return mapTiles; }
	const int getMapWidth() const { return levelWidth; }
	const int getMapHeight() const { return levelHeight; }
	const int isLevelFinished() const { return _levelFinished; }
	bool update(std::shared_ptr<CWorldObject> playerObject, 
		std::vector<std::shared_ptr<CWorldObject>> &worldObject);
};