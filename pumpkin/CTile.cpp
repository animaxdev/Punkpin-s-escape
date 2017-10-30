#include "CTile.h"

CTile::CTile(int a, int b, typeOfTile what)
{
	x = a; collider.x = static_cast<int>(x + 40);
	y = b;	collider.y = static_cast<int>(y + 40);
	collider.w = tileSize - 16;
	collider.h = tileSize - 16;
	objectType = TILE;
	tileType = what;
	eventID = 0;

	switch (tileType)
	{
	case GROUND: passable = true; break;
	case WALL:	passable = false; break;
	}
}