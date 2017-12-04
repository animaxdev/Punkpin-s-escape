#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iterator>
#include <SDL2/SDL.h>
#include <algorithm>
#include "CTile.h"
#include "sqlite3ppext.h"
#include "sqlite3pp.h"
#include "defines.h"
//#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>
#include <boost/bind.hpp>

using namespace boost::filesystem;


class CTools
{
private:
    CTools() = default;
    ~CTools() {}
    int levelNr;
    std::vector<enemyIdData> idData;
    std::vector<weaponIdData> weapon_idData;
    int getLastEventId();

public:

    static CTools & getInstance()
    {
        static CTools * instance = new CTools();
        return *instance;
    }

    //  Those funcs surely need some clarity...
    //  ...and they will get it sometime
    //
    std::vector<directory_entry> readFilenames(std::string dir);
    std::vector<std::vector<char>> readMap(std::string file);

    // writers actual map to .map file
    void writeMapData(
        const std::vector< std::vector< std::shared_ptr< CTile >>> &tiles);

    // reads events data for current level
    std::vector<event_data> getEventsData(unsigned levelNr);

    // writes data from editor
    void writeEventsData(unsigned levelNr);

    // writes current enemy position & id to db
    void writeEnemy(int eventID, std::string posX, std::string posY, unsigned enemyID);

    // writes current enemy position & id to db
    void writeItem(int eventID, std::string posX, std::string posY, unsigned ID);

    // writes current event to db
    void writeEvent(int mapID, unsigned posX, unsigned posY, std::string message);

    // level editor
    void enableLevelEditor_(const Uint8* currentKeyStates,
        int mouseX, int mouseY,
        const std::vector< std::vector< std::shared_ptr< CTile >>> &tiles,
        int level);

    void loadData();
    //const enemyIdData & idData_by_id(int id) const;

    const std::vector<enemyIdData> & getEnemyData() { return idData; }
    const std::vector<weaponIdData> & getWeaponData() { return weapon_idData; }

    template <typename T>
    const T & GetDataByID(int id, std::vector<T> data) const
    {
        auto dat = std::find_if(data.begin(), data.end(),
                                boost::bind(&T::id, _1) == id);
        return *dat;
    }
};
