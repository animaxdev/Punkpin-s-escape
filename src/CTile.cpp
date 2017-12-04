#include "CTile.h"

CTile::CTile(int a, int b, typeOfTile what)
{
    x = a;  collider.x = static_cast<int>(x+42);
    y = b;	collider.y = static_cast<int>(y+42);
    collider.w = tileSize;
    collider.h = tileSize;
	objectType = TILE;
	tileType = what;

	switch (tileType)
	{
	case GROUND: passable = true; break;
	case WALL:	passable = false; break;
	}
}

bool CTile::update()
{
    wasHit = false;
    return true;
}

void CTile::onHit()
{
    wasHit = true;
}
