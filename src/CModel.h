#pragma once

#include "CWorldObject.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CBullet.h"
#include "CEnemy.h"
#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <algorithm>
#include "CGameEventManager.h"



//  This class keeps all game data and handles all game updates
//  game data consist of:
//      - player object
//      - world objects (entities, bullets)
//      - game events (which are world objects aswell, but
//           i decided to store them separetely in a gameEventManager
//      - currentMessage - string that is shown onto screen
//           when's not null
//      - mapObject - stores tiles and manages them
//
//  MAIN GAME MODEL UPDATE LOOP:   worldUpdate()
//
//
//  Note:
//  Collision detection and interaction habits are described in worldUpdate() function



//  Vector of shared pointers to CWorldObject
typedef std::vector<std::shared_ptr<CWorldObject>> world_objects;


class CModel final : public std::enable_shared_from_this<CModel>
{
private:

    world_objects worldObject;
    int levelNr;
    std::shared_ptr<CPlayer> playerObject = std::make_shared<CPlayer>();
    std::shared_ptr<CMap> mapObject = std::make_shared<CMap>("data/map/map_0.map");
    std::unique_ptr<CGameEventManager> gameEventManager = std::make_unique<CGameEventManager>(levelNr);
    std::string currentMessage;
    long frameNr;

    bool _isPaused;
    bool cleanUp();
    bool killAll();
    bool update();


public:

    CModel();
    ~CModel();

    void status();
    bool worldUpdate();
    bool addBullet(typeObj, int x = 0, int y = 0);
    
    const world_objects getWorldObjects() const { return worldObject; }
    std::shared_ptr<CPlayer> getPlayerObject() const {  return playerObject;    }
    std::shared_ptr<CMap> getMapObject() const { return mapObject; }
    std::shared_ptr<CModel> getModel() { return shared_from_this(); }
    const std::string getCurrentMessage() { return currentMessage; }
    int getLevelNr() const { return levelNr; }
    long getFrameNr() const { return frameNr; }
    bool isPaused() const { return _isPaused; }
    void togglePause();
    bool reset();

    friend std::ostream& operator<<(std::ostream& stream, const CModel & model);
};
