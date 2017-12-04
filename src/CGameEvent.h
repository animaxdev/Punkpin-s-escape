#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <utility>
#include "CEnemy.h"
#include "CTools.h"




class CGameEvent : public CWorldObject
{
private:

    //  message displayed at event occurance
    std::string eventMessage;

    std::vector<enemyData> enemy_data;
    std::vector<itemData> item_data;

public:

    CGameEvent(position, const int, std::string);

    CGameEvent(position, const int, std::vector<enemyData>,
               std::vector<itemData>, std::string);

	~CGameEvent() { } // for debugging: { std::cout << "EVENT DESTROYED\n"; }

	bool update() { return true; } //todo

    std::vector<enemyData> & getEnemyData() { return enemy_data; }
    std::vector<itemData> & getItemData() { return item_data; }
    const std::string & getEventMessage() { return eventMessage; }
};
