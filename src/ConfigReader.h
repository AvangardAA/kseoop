/*
 * ConfigReader.h
 *
 *  Created on: Nov 23, 2023
 *      Author: artem
 */

#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include "includes.h"
#include "Airplane.h"

class ConfigReader
{
public:
	ConfigReader(const std::string& configFile);

	std::vector<Airplane>* getPlaneBuffer()
	{
		return &planeRoutesBuffer;
	}

protected:
	std::vector<Airplane> planeRoutesBuffer = {};

};

#endif /* CONFIGREADER_H_ */
