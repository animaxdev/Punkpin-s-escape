#pragma once

#include "CGameEvent.h"
#include "CPlayer.h"

typedef std::vector<std::shared_ptr<CGameEvent>> game_events;

class CGameEventManager
{
private:
	game_events gameEvents;
	std::string pathToFile;

public:
	CGameEventManager(std::string path);
	~CGameEventManager();

	bool setEventManager();
	
	std::vector<std::shared_ptr<CGameEvent>> getGameEvents() { return gameEvents; }
	bool cleanUp();

	//	player vs event interactions
	bool update(const std::shared_ptr<CPlayer> playerObject, 
		std::vector<std::shared_ptr<CWorldObject>> &worldObject);

};