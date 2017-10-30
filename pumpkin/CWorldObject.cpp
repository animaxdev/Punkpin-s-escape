#include "CWorldObject.h"

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
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of *this*
	leftA = this->collider.x;
	rightA = this->collider.x + this->collider.w;
	topA = this->collider.y;
	bottomA = this->collider.y + this->collider.h;

	//Calculate the sides of *obj*
	leftB = obj->getCollider().x;
	rightB = obj->getCollider().x + obj->getCollider().w;
	topB = obj->getCollider().y;
	bottomB = obj->getCollider().y + obj->getCollider().h;

	//If any of the sides from *this* are outside of *obj*
	if (bottomA <= topB)
		return false;

	if (topA >= bottomB)
		return false;

	if (rightA <= leftB)
		return false;

	if (leftA >= rightB)
		return false;

	//If none of the sides from *this* are outside *obj*
	return true;
}

bool CWorldObject::setDestinationCords(const std::shared_ptr<CWorldObject> obj)
{
	xDest = obj->getX();
	yDest = obj->getY();
	return true;
}

void CWorldObject::moveBack()
{
	x -= xVelocity; 
	collider.x = static_cast<int>(x);
	y -= yVelocity; 
	collider.y = static_cast<int>(y);
}

/*bool CWorldObject::isOutOfBounds(int, int) const
{
	
}*/
