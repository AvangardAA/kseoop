/*
 * ConfigReader.cpp
 *
 *  Created on: Nov 23, 2023
 *      Author: artem
 */

#include "ConfigReader.h"

ConfigReader::ConfigReader(const std::string& configFile)
{
	std::ifstream fileInput{configFile};
	std::string buf;

	while (std::getline(fileInput, buf))
	{
		planeRoutesBuffer.push_back(Airplane(buf));
	}
}
