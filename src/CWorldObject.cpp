#include "CWorldObject.h"

CWorldObject::CWorldObject()
{
    collider.w = TEXTURE_SIZE / HITBOX_RATIO;
    collider.h = TEXTURE_SIZE / HITBOX_RATIO;
    angle = 0;
    size = 1;
    wasHit = false;
    spawnTime = CTimer::getInstance().getTicks();
}

CWorldObject::CWorldObject(int sz) : CWorldObject()
{
    size = sz;
}

double CWorldObject::calculateAngle(double X, double Y)
{
    double deltaY = Y - getY();
    double deltaX = X - getX();
    angle = atan2(deltaY, deltaX) * 180 / M_PI;
    return angle;
}

bool CWorldObject::checkCollision(const std::shared_ptr<CWorldObject> obj) //const CWorldObject &obj)
{
    //The sides of the rectangles
//  int leftA, leftB;
//  int rightA, rightB;
//  int topA, topB;
//  int bottomA, bottomB;

//  //Calculate the sides of *this*
//  leftA = this->collider.x;
//  rightA = this->collider.x + this->collider.w;
//  topA = this->collider.y;
//  bottomA = this->collider.y + this->collider.h;

//  //Calculate the sides of *obj*
//  leftB = obj->getCollider().x;
//  rightB = obj->getCollider().x + obj->getCollider().w;
//  topB = obj->getCollider().y;
//  bottomB = obj->getCollider().y + obj->getCollider().h;

//  //If any of the sides from *this* are outside of *obj*
//  if (bottomA <= topB)
//      return false;

//  if (topA >= bottomB)
//      return false;

//  if (rightA <= leftB)
//      return false;

//  if (leftA >= rightB)
//      return false;

    double objSize = obj->getSize();
    if(this->size > objSize)
        objSize = this->size;

    if(getDistance(obj) < objSize*36)
        return true;
    else
        return false;
}

bool CWorldObject::setDestinationCords(const std::shared_ptr<CWorldObject> obj)
{
    xDest = obj->getX();
    yDest = obj->getY();
    return true;
}

void CWorldObject::moveBack()
{
    x -= xVelocity*4;
    collider.x = static_cast<int>(x);
    y -= yVelocity*4;
    collider.y = static_cast<int>(y);

//    x = lastX;
//    y = lastY;
//    collider.x = static_cast<int>(x);
//    collider.y = static_cast<int>(y);
}

void CWorldObject::move(double deg, double val)
{
    double radians = deg * (M_PI / 180);
    xVelocity = val*cos(radians);
    yVelocity = val*sin(radians);
    x += xVelocity;
    y += yVelocity;
}

double CWorldObject::getDistance(const std::shared_ptr<CWorldObject> obj)
{
    float dist_a = pow( (x + size*TEXTURE_SIZE/2) - (obj->getX() + obj->getSize()*TEXTURE_SIZE/2), 2);
    float dist_b = pow( (y + size*TEXTURE_SIZE/2) - (obj->getY() + obj->getSize()*TEXTURE_SIZE/2), 2);

    auto distance = sqrt(dist_a + dist_b );
    return distance;
}
