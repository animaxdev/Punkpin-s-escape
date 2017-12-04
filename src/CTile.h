#pragma once

#include "CWorldObject.h"

enum typeOfTile { GROUND, WALL, DOOR, EVENT };

class CTile : public CWorldObject
{

private:

	bool passable;
	typeOfTile tileType;


public:

	CTile(int a, int b, typeOfTile what);
	~CTile() { } //std::cout << "Tile destroyed!\n";

    bool update() override;
    void onHit() override;

	void setTileType(typeOfTile what) { this->tileType = what; }
	typeOfTile getTileType() const { return tileType; }

	static const int tileSize = 64;
};
