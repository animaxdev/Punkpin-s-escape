#include "CModel.h"

CModel::CModel()
{
    levelNr = 0;
    _isPaused = 0;
}

CModel::~CModel()
{
    std::cout << "gameModel destroyed\n";
}

void CModel::status()
{
    system("clear");
    std::cout << *this;
}

bool CModel::worldUpdate()
{
    //  Major player updater
    playerObject->update();

    //  Major game events updater (passes message) - spawns enemies
    currentMessage = gameEventManager->update(playerObject, worldObject);

    //  Map vs worldObjects (+ player) interactions
    mapObject->update(playerObject, worldObject);

    //  update worldObject vs worldObject (+ player) interactions:
    this->update();

    //  Deletions
    cleanUp();

    frameNr++;
    return true;
}

bool CModel::addBullet(typeObj type, int x, int y)
{
    switch (type)
    {
    case BULLET:
        {
            if(playerObject->hasWeapon())
            {
                auto bullets = playerObject->shoot();

                if(bullets.empty() == false)
                {
                    std::copy(bullets.begin(), bullets.end(),
                              std::back_inserter(worldObject));
                }
            }
        }

        break;

    case ENEMY_BULLET:

        worldObject.push_back(std::make_shared<CBullet>(    x,
                                                            y,
                                                            playerObject->getX(),
                                                            playerObject->getY() )
                                                            );
        break;

    default:
        std::cout << "wrong object type while spawning bullet \n";
        break;
        return false;
    }

    return true;
}

bool CModel::cleanUp()
{
    //  Operations that change size of worldObject vector
    std::vector<CWorldObject>::size_type size = worldObject.size();

    for (std::vector<CWorldObject>::size_type i = 0; i < size; ++i)
    {
        //  Remove dead objects
        if (worldObject[i]->isDead && !worldObject[i]->isHit())
        {
            worldObject.erase(worldObject.begin() + i);
            --size;
        }
    }

    //  Remove dead events
    gameEventManager->cleanUp();

    return true;
}

bool CModel::killAll()
{
    this->worldObject.clear();
    return true;
}

bool CModel::update()
{
    for (auto &i : worldObject)
    {
        //  If object is an enemy
        if (i->is(ENEMY))
        {

            //  If enemy is aggressive
            if ( std::static_pointer_cast<CEnemy>(i)->isAggressive() )
            {
                //  Set destination of enemy as player's coordinates
                i->setDestinationCords(playerObject);

                // todo: set route to player (A* perhaps)
                //  if blocked by a wall/box etc.
            }

            i->update();

            //  And check for collision with player
            if (i->checkCollision(playerObject))
            {
                //  if it's a ghost...
                if (std::static_pointer_cast<CEnemy>(i)->hasPlannedRoute())
                    playerObject->isDead = true;
                else                                //  if it's a zombie
                {
                    i->isDead = true;               //  kill enemy
                    playerObject->onHit();          //  hit player
                }
            }

            auto dist = i->getDistance(playerObject);
            std::static_pointer_cast<CEnemy>(i)->handleDistance(dist);

            //  collision for any world object - not really needed, but let it be here
            /*for(auto &j : worldObject)
            {
                if (i != j && i->checkCollision(j))
                {
                    i->moveBack();
                }
            }*/

        }

        //  if object is a bullet or a tile
        else if (i->is(BULLET))
        {
            i->update();

            //  Iterate through objects for collision detection
            for (auto &j : worldObject)
            {
                //  If collided with something
                if (j->is(ENEMY) && i->checkCollision(j))
                {
                    i->isDead = true;                   // Destroy the bullet
                    std::dynamic_pointer_cast<CEnemy>(j)->onHit(std::dynamic_pointer_cast<CBullet>(i));
                    //playerObject->addToScore(1);
                    break;
                }
            }
        }

        //  if object is enemy's bullet
        else if (i->is(ENEMY_BULLET))
        {
            //  Set destination of bullet as player's coordinates
            i->setDestinationCords(playerObject);
            i->update();

            //  If enemy's bullet collided with player object
            if (i->checkCollision(playerObject))
            {
                playerObject->onHit();      //  Hit player
                i->isDead = true;           //  Destroy enemy's bullet
            }
        }

        //  if object is an item
        else if (i->is(ITEM))
        {
            if(i->checkCollision(playerObject))
            {
                auto item = std::dynamic_pointer_cast<CItem>(i);

                //  consumables are score points only for now...
                if( item->isConsumable() )
                {
                    //  so add to score
                    playerObject->addToScore(SCORE_POINT);
                }
                else
                {
                    //  otherwise we have a new weapon
                    auto wep = item->getData();
                    playerObject->addWeapon(wep);
                }
                //  destroy item
                i->isDead = true;
            }
        }
    }

    //  Time based insertions (eg. enemy shoots after desired time)
    std::vector<CWorldObject>::size_type size = worldObject.size();

    for (std::vector<CWorldObject>::size_type i = 0; i < size; ++i)
    {
        if ( worldObject[i]->is(ENEMY) )
        {
            auto enemy = std::dynamic_pointer_cast<CEnemy>(worldObject[i]);

            //  if enemy can shoot - shoot
            if( enemy->canShoot() )
            {
                auto bullets = enemy->shoot();
                for(auto &i : bullets)
                    worldObject.push_back(i);
            }
        }
    }


    // if level finished, change map
    if (mapObject->isLevelFinished())
    {
        this->levelNr++;
        this->reset();
    }

    //  if player's dead, reset current map
    if (playerObject->isDead)
        this->reset();

    //  let user read message before destroyed...
    if(!this->getCurrentMessage().empty())
        this->togglePause();

    return true;
}

bool CModel::reset()
{
    this->killAll();
    CTimer::getInstance().start();
    playerObject->setStartPos();
    playerObject->isDead = false;
    mapObject.reset(new CMap("data/map/map_" + std::to_string(this->levelNr) + ".map"));
    gameEventManager.reset(new CGameEventManager(this->levelNr));
    return true;
}

void CModel::togglePause()
{
    _isPaused = !_isPaused;

    //  If you're pausing - pause the Timer too...
    if (_isPaused)
        CTimer::getInstance().pause();
    else
        CTimer::getInstance().unpause();
}

std::ostream& operator<<(std::ostream & os, const CModel & model)
{
    os << "world objects count: " << model.worldObject.size() << std::endl;
    return os;
}
