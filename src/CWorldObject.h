#pragma once

#define _USE_MATH_DEFINES
//#include "CGame.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <ncurses.h>
#include "defines.h"
#include "CTimer.h"
#include <boost/assign/list_of.hpp>
#include <vector>

typedef std::pair<double, double> coordinates;
enum typeObj { BULLET = 0 , ENEMY_BULLET, ENEMY, TILE, ITEM, UNDEFINED };

class CWorldObject
{
protected:

double x, y, xDest, yDest;
// double lastX, lastY;
double xVelocity, yVelocity;
double angle;
double size;
bool wasHit;
SDL_Rect collider;
typeObj objectType = UNDEFINED;


public:

CWorldObject();
CWorldObject(int);
virtual ~CWorldObject() { } //std::cout << "world object destroyed!\n"; }

//  basics
//
bool isDead = false;        //  tbd...
Uint32 spawnTime;           //  tbd...

// main updates
virtual bool update() = 0;
virtual void onHit() { wasHit = true; }

//  collision/movement interfaces
//
virtual bool setDestinationCords(const std::shared_ptr<CWorldObject> obj);
bool checkCollision(const std::shared_ptr<CWorldObject> obj);
void moveBack();
double calculateAngle(double, double);

//  gets/returns
//
double getX() const { return x; }
double getY() const { return y; }
virtual double getAngle() const { return angle; }
const CWorldObject& getObject() { return *this; }
const SDL_Rect& getCollider() const { return collider; }
bool is(typeObj type) const { return objectType == type; }
bool isHit() const { return wasHit; }

virtual void move(double deg, double val);

double getDistance(const std::shared_ptr<CWorldObject> obj);
//void resetTimer() { spawnTime = 0;}

//virtual void
const double & getAddrX() { return x;}
const double & getAddrY() { return y;}
const double & getSize() const { return size; }

};
