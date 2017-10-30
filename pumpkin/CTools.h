#pragma once

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <SDL.h>
#include "CTile.h"

using namespace boost::filesystem;


class CTools
{
private:
	CTools() = default;
	~CTools() {}
	int levelNr;

public:

	static CTools & getInstance()
	{
		static CTools * instance = new CTools();
		return *instance;
	}

	//	Those funcs surely need some clarity...
	//	...and they will get it sometime
	//
	std::vector<directory_entry> readFilenames(std::string dir);
	std::vector<std::vector<char>> readMap(std::string file);
	std::vector<std::vector<std::pair<double, double>>> readMapPopulation(std::string file);
	std::vector<std::vector<std::vector<std::pair<double, double>>>> CTools::readMapPopulation_(std::string file);

	void writeMapData(std::vector<std::vector<std::string>> pos,
		const std::vector< std::vector< std::shared_ptr< CTile >>> &tiles);

	void enableLevelEditor(const Uint8* currentKeyStates,
		int mouseX, int mouseY,
		const std::vector< std::vector< std::shared_ptr< CTile >>> &tiles,
		int level);

};