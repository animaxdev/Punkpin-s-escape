#pragma once

#include <vector>
#include <memory>

// it's more handy for now than class members...
// ...
// ...i know, i know.



#define SCREEN_WIDTH        1024
#define SCREEN_HEIGHT       768
#define TEXTURE_SIZE        64

#define	LEVEL_WIDTH         1216
#define LEVEL_HEIGHT        2240

// bigger ratio - smaller hitbox
#define HITBOX_RATIO        1.5
#define EVENT_HITBOX_SIZE   3

#define FRAMERATE           60

#define MAPS_DIR            "data/map/"
#define	DAT_DIR             "data/dat/"

#define NEWLINE_SIZE        25
#define PTEXT_POS           50

#define EDITOR_ENABLED      0
#define HP_BAR_TRESHOLD     100
#define SCORE_POINT         5
#define COLLISION_DISTANCE  36




typedef std::pair<double,double> position;

//	Each position has coordinates so we have a vector of coordinates
//	for each enemy, so they are able to move through destinations (see CEnemy.h)
typedef std::vector<std::vector<std::pair<double, double> > > spawnPositions;


// x's and y's
typedef std::vector<position> enemyPositions;

// enemy positions and enemy_id
typedef std::pair<enemyPositions, int> enemyData;
typedef std::pair<position, int> itemData;

// eventData consisting of enemy positions, enemy_id,
// event message and position
struct eventData {
    std::vector<enemyData> enemy_data;
    position event_position;
    std::string event_message;
};


typedef std::vector<position> positions;

struct event_data {
    std::vector<itemData> item_data;
    std::vector<enemyData> enemy_data;
    position event_position;
    std::string event_message;
};

// enemyIdData consisting of data for
// each enemy ID
struct enemyIdData {
    int id;
    int hp;
    int velocity;
    bool aggro;
    bool can_aggro;
    int weapon_id;
    std::string name;
    int static_angle;
    int aggro_radius;
    float size;
};

// struct holdiing data for a bullet
//
struct bullet_data {
    int damage;
    int recoil;
    int speed;
};

// struct holding data for a weapon
//
struct weaponIdData {
    int id;
    bullet_data bulletData;
    int reloadTime;
    int bulletCountMax;
    int shootsAtOnce;
    std::string name;
    int spreadFactor;
};



struct cameraPosition { int x; int y; };

const static std::vector<std::string> playerMessages{"Ouch!",
                                        "Bloody monster!",
                                        "That really hurt!",
                                        "Why are you doing/this to me?",
                                        "Stop it!",
                                        "Aaaaaaaa!",
                                        "God dammit!",
                                        "You'll regret it/you stinky creature!" };

//TODO
//const static std::vector<std::string> enemyMessages{"Arrghh!!",
//                                                   "Brains!",
//                                                   "Fresh blood!",
//                                                   "*indistinguishable gibberish*",
//                                                   "Rrroooar",
//                                                   "Bad and mean-o/ugly pumpkin-o",
//                                                   "Gonna eat ya, haha!"};
