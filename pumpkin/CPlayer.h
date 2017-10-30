#pragma once

#include "CWorldObject.h"

class CPlayer final : public CWorldObject
{
private:
	int velocity = 4;
	
	double angle;
	int healthPoints;
	unsigned int scorePoints;

	/*template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}*/

public:
	CPlayer();
	~CPlayer() { std::cout << "PLAYER DESTROYED\n"; }
	bool move(std::string);
	bool update() override;

	void onHit();
	void addToScore(unsigned int);
	unsigned int getScore() const;
	const int getHealthPoints() const { return healthPoints; }
	void setVelocity(int val) { velocity = val; }
	void setStartPos() { x = 100, y = 100; healthPoints = 5; }
};
