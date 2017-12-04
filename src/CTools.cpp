#include "CTools.h"

std::vector<directory_entry> CTools::readFilenames(std::string dir)
{
	path p(dir);
	std::vector<directory_entry> v;

	try
	{
		if (exists(p))    // does p actually exist?
		{
			if (is_regular_file(p))        // is p a regular file?   
				std::cout << p << " size is " << file_size(p) << '\n';

			else if (is_directory(p))      // is p a directory?
			{
				std::cout << p << " is a directory containing:\n";
				std::copy(directory_iterator(p), directory_iterator(), back_inserter(v));

				for (auto &i : v)
					std::cout << i.path().string() << std::endl;
			}
			else
				std::cout << p << " exists, but is neither a regular file nor a directory\n";
		}
		else
			std::cout << p << " does not exist\n";
	}

	catch (const filesystem_error& ex)
	{
		std::cout << ex.what() << '\n';
	}
	return v;
}

std::vector<std::vector<char>> CTools::readMap(std::string file)
{
	std::vector<std::vector<char>> map;

	char temp = NULL;
	int numberOfLines = 0;
	std::ifstream fin;
	fin.open(file, std::ios::in);
	map.resize(1);

	while (!fin.eof()) {

		fin.get(temp);
		if (temp == '\n')
		{
			++numberOfLines; 
			map.resize(numberOfLines+1);
		}
		else
		{
			map[numberOfLines].push_back(temp);
		}
	}

	return map;
}

void CTools::writeMapData(const std::vector<std::vector<std::shared_ptr<CTile>>> & tiles)
{
	std::ofstream file;
	
	file.open("data/map/map_"+std::to_string(levelNr)+".map");

	for (int i = 0; i < tiles[0].size(); i++)
	{
		for (int j = 0; j < tiles.size(); j++)
		{
			switch (tiles[j][i]->getTileType())
			{
			case GROUND:
				file << 'g'; break;
			case WALL:
				file << 'w'; break;
			case DOOR:
				file << 'd'; break;
			}
		}

		if (i < tiles[0].size() - 1)
			file << '\n';
	}

	file.close();
}

std::vector<event_data> CTools::getEventsData(unsigned levelNr)
{
    sqlite3pp::database db("data/data.db");
    std::vector<eventData> tempEventData;
    std::vector<event_data> temp_EventData;
    std::string message;
    std::vector<std::string> strs_x, strs_y;
    std::string xPos, yPos;

    std::string q = "SELECT * FROM events WHERE map_id=" + std::to_string(levelNr);
    sqlite3pp::query qry(db, q.c_str());

    // for each EVENT in database coresponding to current map
    for (auto v : qry)
    {
          std::vector<enemyData> tempEnemyData;
          std::vector<itemData> tempItemData;
          int pos_x, pos_y, map_id, event_id, enemy_id;

          // get event data from db
          v.getter() >> map_id >> event_id >> pos_x >> pos_y >> message;

          std::string q = "SELECT * FROM enemy_pos WHERE event_id="+std::to_string(event_id);
          sqlite3pp::query qry(db, q.c_str());

          // for each ENEMY in event
          for(auto a : qry)
          {
              enemyPositions tempPos;

              // get enemy positions and id
              a.getter() >> sqlite3pp::ignore >> xPos >> yPos >> enemy_id;

              // two vectors of positions - need to transpose
              boost::split(strs_x, xPos, boost::is_any_of(","));
              boost::split(strs_y, yPos, boost::is_any_of(","));

              // now transpose position data...
              auto vectorSize = strs_x.size();
              for(int i = 0; i<vectorSize; ++i)
              {
                  // boost::lexical_cast<double>    is quite slow...
                  std::pair<double, double> pairToPush = {std::stod(strs_x[i]), std::stod(strs_y[i])};
                  tempPos.push_back(pairToPush);
              }

              // and add them if not empty
              if(tempPos.empty() == false && enemy_id >= 0)
                 tempEnemyData.push_back({tempPos, enemy_id});
          }

          q = "SELECT * FROM item_pos WHERE event_id="+std::to_string(event_id);
          sqlite3pp::query wqry(db, q.c_str());

          for(auto a : wqry)
          {
              itemData temp;
              a.getter() >> sqlite3pp::ignore >> temp.first.first >> temp.first.second >> temp.second;
              tempItemData.push_back(temp);
          }

          std::pair<double,double> event_pos = {pos_x, pos_y};
          //tempEventData.push_back({ tempEnemyData, event_pos, message });

          temp_EventData.push_back({ tempItemData, tempEnemyData, event_pos, message});
    }

    return temp_EventData;
}

void CTools::enableLevelEditor_(const Uint8 * currentKeyStates, int mouseX, int mouseY, const std::vector<std::vector<std::shared_ptr<CTile>>>& tiles, int level)
{
    this->levelNr = level;

    static bool mode = 0;
    static int eventId = 0;
    static int objectId = 1;
    static int eventX = 0;
    static int eventY = 0;
    static std::string message = "";
    static std::string strs_x, strs_y;
    static std::vector<std::string> itemPosX;
    static std::vector<std::string> itemPosY;

    int currX = (mouseX - (mouseX % CTile::tileSize)) / CTile::tileSize;
    int currY = (mouseY - (mouseY % CTile::tileSize)) / CTile::tileSize;

    if (currentKeyStates[SDL_SCANCODE_G])			// write pos to spawn enemy
    {
        if(mode)
        {
            itemPosX.push_back(std::to_string(mouseX));
            itemPosY.push_back(std::to_string(mouseY));
        }
        else
        {
            if(!strs_x.empty() && !strs_y.empty()) {
                strs_x += ",";
                strs_y += ",";
            }

            strs_x += std::to_string(mouseX);
            strs_y += std::to_string(mouseY);
        }


    }
    if (currentKeyStates[SDL_SCANCODE_H])			// push position set
    {
        //eventId = getLastEventId();

        std::cout << "\nEnter eventId:\n";
        std::cin >> eventId;


        if(mode && !itemPosX.empty())
        {
            for(int i = 0; i < itemPosX.size(); ++i)
            {
                writeItem(eventId, itemPosX[i], itemPosY[i], objectId);
            }
        }

        else if(!strs_x.empty() && !strs_y.empty() && eventId > -1 && objectId > -1) {
                writeEnemy(eventId, strs_x, strs_y, objectId);
        }

        strs_x.clear();
        strs_y.clear();
        itemPosX.clear();
        itemPosY.clear();

    }
    if (currentKeyStates[SDL_SCANCODE_J])			// clear position buffer
    {
        strs_x.clear();
        strs_y.clear();
    }
    if (currentKeyStates[SDL_SCANCODE_Q])			// add wall at cursor's pos
    {
        tiles[currX][currY]->setTileType(WALL);
    }
    if (currentKeyStates[SDL_SCANCODE_E])			// add ground at cursor's pos
    {
        tiles[currX][currY]->setTileType(GROUND);
    }
    if (currentKeyStates[SDL_SCANCODE_Z])			// add door at cursor's pos
    {
        tiles[currX][currY]->setTileType(DOOR);
    }
    if (currentKeyStates[SDL_SCANCODE_M])			// write map
    {
        this->writeMapData(tiles);
    }
    if (currentKeyStates[SDL_SCANCODE_1])			// set ID, default = 0;
    {
        std::cout << "\nEnter objectId:\n";
        std::cin >> objectId;
    }
    if (currentKeyStates[SDL_SCANCODE_2])			// set event
    {
        eventX = mouseX;
        eventY = mouseY;

        if(eventY!=0 && eventX != 0)
        {
            this->writeEvent(levelNr, eventX, eventY, message);
            message = "";
            eventX = 0;
            eventY = 0;
        }
    }
    if (currentKeyStates[SDL_SCANCODE_3])			// enter message
    {
        std::cout << "\nEnter event message:\n";
        std::getline(std::cin, message);
    }
    if (currentKeyStates[SDL_SCANCODE_4])			// change mode
    {
        mode = !mode;
    }

    system("clear");
    std::cout << "|**************** EDITOR ****************|" << std::endl;
    std::cout << "Mode: " << mode << std::endl;
    std::cout << "Message: " << message << std::endl;
    std::cout << "Pos: (" << mouseX << ", " << mouseY << ")" << std::endl;
    std::cout << "ID: " << objectId << std::endl;
    std::cout << "|****************************************|" << std::endl;

}

void CTools::writeEnemy(int eventID, std::string posX, std::string posY, unsigned enemyID)
{
    sqlite3pp::database db("data/data.db");
    sqlite3pp::command cmd(db, "INSERT INTO enemy_pos VALUES (?, ?, ?, ?)");
    cmd.binder() << std::to_string(eventID) << posX << posY << std::to_string(enemyID);
    cmd.execute();
}

void CTools::writeItem(int eventID, std::string posX, std::string posY, unsigned ID)
{
    sqlite3pp::database db("data/data.db");
    sqlite3pp::command cmd(db, "INSERT INTO item_pos VALUES (?, ?, ?, ?)");
    cmd.binder() << std::to_string(eventID) << posX << posY << std::to_string(ID);
    cmd.execute();
}

void CTools::writeEvent(int mapID, unsigned posX, unsigned posY, std::string message)
{
    sqlite3pp::database db("data/data.db");
    sqlite3pp::command cmd(db, "INSERT INTO events (map_id, pos_x, pos_y, message) VALUES (?, ?, ?, ?)");
    cmd.binder() << std::to_string(mapID) << std::to_string(posX) << std::to_string(posY) << message;
    cmd.execute();
}

int CTools::getLastEventId()
{
    int val;
    sqlite3pp::database db("data/data.db");
    std::string q = "SELECT max(event_id) from events";
    sqlite3pp::query qry(db, q.c_str());

    for(auto v : qry)
        v.getter() >> val;

    return val;
}

void CTools::loadData()
{
    sqlite3pp::database db("data/data.db");
    std::string q = "SELECT * FROM enemies";
    sqlite3pp::query qry(db, q.c_str());

    enemyIdData temp;
    for(auto q : qry)
    {
        q.getter() >> temp. id >> temp.hp >> temp.velocity >>
                      temp.aggro >> temp.can_aggro >> temp.weapon_id >>
                      temp.name >> temp.static_angle >> temp.aggro_radius >> temp.size;

        this->idData.push_back(temp);
    }

    q = "SELECT * FROM weapons";
    sqlite3pp::query wqry(db, q.c_str());

    weaponIdData wtemp;
    for(auto q : wqry)
    {
        q.getter() >> wtemp.id >> wtemp.bulletData.damage >> wtemp.bulletData.recoil >>
                      wtemp.bulletData.speed >> wtemp.reloadTime >> wtemp.bulletCountMax >>
                      wtemp.shootsAtOnce >> wtemp.spreadFactor >> wtemp.name ;

        this->weapon_idData.push_back(wtemp);
    }
}
