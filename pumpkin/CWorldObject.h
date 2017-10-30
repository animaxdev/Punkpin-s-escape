#pragma once

#define _USE_MATH_DEFINES
//#include "CGame.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <SDL.h>
#include <conio.h>
#include "defines.h"
#include "CTimer.h"
#include <boost/assign/list_of.hpp>
#include <vector>

typedef std::pair<double, double> coordinates;
enum typeObj { BULLET = 0 , ENEMY_BULLET, ENEMY, TILE, UNDEFINED };

class CWorldObject
{
protected:
	//coorinates destination;		<- tbd - all bound vars to cords
	double x, y, xDest, yDest;
	double xVelocity, yVelocity;
	double angle;
	SDL_Rect collider;
	typeObj objectType = UNDEFINED;

public:
	CWorldObject() {
		collider.w = 64;
		collider.h = 64; 
		angle = 0;
	};
	virtual ~CWorldObject() = default;

	//	basics
	//
	bool isDead = false;		//	tbd...
	Uint32 spawnTime;			//	tbd...
	virtual bool move() { std::cout << "unit can't move.\n";  return false; }
	virtual bool update() = 0;
	

	//	collision/movement interfaces
	//
	bool checkCollision(const std::shared_ptr<CWorldObject> obj);
	virtual bool setDestinationCords(const std::shared_ptr<CWorldObject> obj);
	void moveBack();
	double calculateAngle(double, double);

	//	gets/returns
	//
	const double getX() const { return x; }
	const double getY() const { return y; }
	double getAngle() const { return angle; }
	const CWorldObject& getObject() { return *this; }
	bool is(typeObj type) const { return objectType == type; }
	const SDL_Rect& getCollider() const { return collider; }
};