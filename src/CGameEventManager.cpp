#include "CGameEventManager.h"

CGameEventManager::CGameEventManager(int level)
{
    levelNr=level;
    this->setEventManager();
}

CGameEventManager::~CGameEventManager()
{
    std::cout << "Game Event Manager DESTROYED\n";
}

bool CGameEventManager::setEventManager()
{
    auto data = CTools::getInstance().getEventsData(this->levelNr);

    for(auto &i : data)
    {
        gameEvents.push_back(std::make_shared<CGameEvent>(i.event_position,
                                                          EVENT_HITBOX_SIZE,
                                                          i.enemy_data,
                                                          i.item_data,
                                                          i.event_message));
    }

    return true;
}

bool CGameEventManager::cleanUp()
{
    std::vector<CWorldObject>::size_type size = gameEvents.size();

    for (std::vector<CWorldObject>::size_type i = 0; i < size; ++i)
    {
        //  Remove dead objects
        if (gameEvents[i]->isDead)
        {
            gameEvents.erase(gameEvents.begin() + i);
            --size;
        }
    }
    return true;
}

std::string CGameEventManager::update(const std::shared_ptr<CPlayer> playerObject,
                                      std::vector<std::shared_ptr<CWorldObject>> &worldObject)
{
    for (auto &i : gameEvents)
    {
        // if player collided with event spawn enemies
        // and return eventMessage
        if (i->checkCollision(playerObject) == true)
        {
            // populate
            for (auto &j : i->getEnemyData())
            {
                auto dat = CTools::getInstance().GetDataByID(j.second, CTools::getInstance().getEnemyData());

                //  spawn enemy @ID from data loaded from DB
                auto temp = std::make_shared<CEnemy>(dat, j.first);

                worldObject.push_back(temp);
            }

            for(auto &j : i->getItemData())
            {
                auto dat = CTools::getInstance().GetDataByID(j.second, CTools::getInstance().getWeaponData());

                //  spawn item @ID from data loaded from DB
                auto temp = std::make_shared<CItem>(dat, j.first);
                worldObject.push_back(temp);
            }

            // kill event
            i->isDead = true;

            // return event message
            return ( i->getEventMessage() );
        }
    }
    return "";
}
