#include "CItem.h"


CItem::CItem(position pos) : CWorldObject()
{
    objectType = ITEM;
    x = pos.first;
    y = pos.second;
    consumable = true;
    value = 1;
    data = {};
    collider.x = x; collider.y = y;
}

CItem::CItem(weaponIdData _data, position pos) : CItem(pos)
{
    data = _data;

    if(data.id == 0)
    {
        consumable = true;
        value = 1;
    }
    else
        consumable = false;
}
