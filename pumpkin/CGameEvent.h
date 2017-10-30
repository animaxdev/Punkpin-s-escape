#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <utility>
#include "CEnemy.h"
#include "CTools.h"

//	Each position has coordinates
//	so we have a vector of coordinates
//	for each enemy, so they are able to move
//	through destinations (see CEnemy.h)
typedef std::vector<std::vector<std::pair<double, double> > > spawnPositions;

class CGameEvent : public CWorldObject
{
private:

	//	size of event object
	const int size;

	//	holds enemy positions to spawn
	spawnPositions enemyPositions;

public:

	CGameEvent(const int xPos, const int yPos, const int sz, spawnPositions pos) :
		size(sz), enemyPositions(pos) 
	{
		x = xPos;
		y = yPos;
		collider.x = xPos;	collider.w = size * 64;
		collider.y = yPos;	collider.h = size * 64;
	};

	~CGameEvent() { } // for debugging: { std::cout << "EVENT DESTROYED\n"; }

	bool update() { return true; } //todo
	spawnPositions & getEnemyPositions() { return enemyPositions; }
};