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
	ifstream fin;
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

std::vector<std::vector<std::pair<double, double>>> CTools::readMapPopulation(std::string file)
{
	std::vector<std::vector<std::pair<double, double>>> map;

	char temp = NULL;
	int numberOfLines = 0;
	std::string temp_int = "";
	std::pair<double, double> temp_pair;
	ifstream fin;
	fin.open(file, std::ios::in);
	//map.resize(1);

	while (!fin.eof()) {

		fin.get(temp);
		if (temp == '\n')
		{
			++numberOfLines;
		}
		else if (temp == ',')
		{
			map.resize(numberOfLines + 1);
			temp_pair.first = std::stod(temp_int);
			temp_int = "";
		}
		else if (temp == ';')
		{
			temp_pair.second = std::stod(temp_int);
			map[numberOfLines].push_back(temp_pair);
			temp_int = "";
		}
		else
			temp_int += temp;
	}
	return map;
}

//	3d vector of pairs
std::vector<std::vector<std::vector<std::pair<double, double>>>> CTools::readMapPopulation_(std::string file)
{
	//	2d vector of pairs
	std::vector<std::vector<std::vector<std::pair<double, double>>>> map;

	char temp = NULL;
	int numberOfLines = 0;
	int numberOfEvents = 0;
	std::string temp_int = "";
	std::pair<double, double> temp_pair;
	ifstream fin;
	fin.open(file, std::ios::in);
	//map.resize(1);

	while (!fin.eof()) {

		fin.get(temp);
		if (temp == '#')
		{
			numberOfLines = 0;
			numberOfEvents++;
			map.resize(numberOfEvents);
		}
		else if (temp == '\n')
		{
			++numberOfLines;
		}
		else if (temp == ',')
		{
			map[numberOfEvents-1].resize(numberOfLines + 1);
			temp_pair.first = std::stod(temp_int);
			temp_int = "";
		}
		else if (temp == ';')
		{
			if (temp_int != "")
			{
				temp_pair.second = std::stod(temp_int);
				map[numberOfEvents - 1][numberOfLines].push_back(temp_pair);
				temp_int = "";
			}
		}
		else
			temp_int += temp;
	}
	return map;
}

void CTools::writeMapData(std::vector<std::vector<std::string>> pos, const std::vector<std::vector<std::shared_ptr<CTile>>>& tiles)
{
	ofstream file;
	file.open("data/dat/dat_"+std::to_string(levelNr)+".dat");
	for (auto &i : pos)
	{
		file << "#";
		for (auto &j : i)
		{
			file << j + "\n";
		}
	}
	file.close();
	
	file.open("data/map/map_"+std::to_string(levelNr)+".map");
	int id = 0;
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

void CTools::enableLevelEditor(const Uint8 * currentKeyStates, int mouseX, int mouseY, const std::vector<std::vector<std::shared_ptr<CTile>>>& tiles, int level)
{
	this->levelNr = level;
	static bool mode = 0;

	static std::string temp;
	static std::vector<std::string> positionsToWrite;

	static int enemy_speed = 2;
	static int enemy_hp = 2;

	static std::vector<std::vector<std::string>> eventsToWrite;

	int currX = (mouseX - (mouseX % CTile::tileSize)) / CTile::tileSize;
	int currY = (mouseY - (mouseY % CTile::tileSize)) / CTile::tileSize;

	int eventX = 0;
	int eventY = 0;

	if (currentKeyStates[SDL_SCANCODE_G])			// write pos to spawn enemy
	{
		temp += std::to_string(mouseX)
			+ "," + std::to_string(mouseY) + ";";

		if (!mode) {
			positionsToWrite.push_back(temp);
			temp = "";
		}

		mode = true;
	}
	if (currentKeyStates[SDL_SCANCODE_H])			// push position set
	{
		if (!temp.empty())
			positionsToWrite.push_back(temp);
		temp = "";
	}
	if (currentKeyStates[SDL_SCANCODE_J])			// write to event buffer
	{
		if (!positionsToWrite.empty()) {
			eventsToWrite.push_back(positionsToWrite);
			positionsToWrite.clear();
			mode = 0;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_K])			// pop previous position set
	{
		if (!positionsToWrite.empty())
			positionsToWrite.pop_back();
		else if (!eventsToWrite.empty())
			eventsToWrite.pop_back();
	}
	if (currentKeyStates[SDL_SCANCODE_Q])			// add wall at cursor's pos
	{
		tiles[currX][currY]->setTileType(WALL);
	}
	if (currentKeyStates[SDL_SCANCODE_E])			// add ground at cursor's pos
	{
		//	same as beneath
		tiles[currX][currY]->setTileType(GROUND);
	}
	if (currentKeyStates[SDL_SCANCODE_Z])			// add door at cursor's pos
	{
		tiles[currX][currY]->setTileType(DOOR);
	}
	if (currentKeyStates[SDL_SCANCODE_M])			// write events and map
	{
		writeMapData(eventsToWrite, tiles);
	}

	system("CLS");
	std::cout << "xTile: " << currX << "   yTile: " << currY << std::endl;

	if (mode)
		std::cout << "press: \nG to add position\nH to add buffered positions to event buffer\nK to delete last position\n";
	else
		std::cout << "press: \nG to add event at mouse location\n\n";

	std::cout << "Buffered positions/destination lists: " << std::endl;

	for (auto &i : positionsToWrite)
		std::cout << "# " << i << std::endl;

	std::cout << std::endl << std::endl << "Buffered events: " << std::endl;

	for (auto &i : eventsToWrite)
	{
		for (auto &j : i)
		{
			std::cout << "# " << j << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl << "Current: " << temp << std::endl;
}