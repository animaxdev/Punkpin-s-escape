#include "CGameEvent.h"

CGameEvent::CGameEvent(     position pos,
                            const int sz,
                            std::string mes = ""    ) : CWorldObject(sz)
{
    x = pos.first;
    y = pos.second;
    collider.x = x;	collider.w = size * 64;
    collider.y = y;	collider.h = size * 64;
    eventMessage = mes;
}

CGameEvent::CGameEvent(    position pos,
                           const int sz,
                           std::vector<enemyData> en_data,
                           std::vector<itemData> i_data,
                           std::string mes = ""     ) : CGameEvent(pos, sz, mes)
{
    enemy_data = en_data;
    item_data = i_data;
}


