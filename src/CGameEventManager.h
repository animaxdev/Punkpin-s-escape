#pragma once

#include "CGameEvent.h"
#include "CPlayer.h"
#include "CItem.h"

typedef std::vector<std::shared_ptr<CGameEvent>> game_events;

class CGameEventManager
{
private:
	game_events gameEvents;
    int levelNr;

public:
    CGameEventManager(int level);
    ~CGameEventManager();

    bool setEventManager();
	
    const std::vector<std::shared_ptr<CGameEvent>> getGameEvents() { return gameEvents; }
    bool cleanUp();

    //	player vs event interactions
    std::string update(const std::shared_ptr<CPlayer> playerObject,
        std::vector<std::shared_ptr<CWorldObject>> &worldObject);

};
