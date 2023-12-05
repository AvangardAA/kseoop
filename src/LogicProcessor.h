/*
 * LogicProcessor.h
 *
 *  Created on: Nov 23, 2023
 *      Author: artem
 */

#ifndef LOGICPROCESSOR_H_
#define LOGICPROCESSOR_H_

#include "includes.h"
#include "ConfigReader.h"
#include "Airplane.h"

class LogicProcessor {
public:
	LogicProcessor(ConfigReader& conf);

	std::vector<Airplane>* planeRoutesBufferPtr;

	std::vector<std::pair<char, int>> ListFreePlaces(const std::string& date, const std::string& flightnum);
	bool checkFreePlace(const std::string& date, const std::string& flightnum, const char& place, const int& seatnum);
	std::string addTicket(const std::string& date, const std::string& flightnum, const Ticket& tick);
	bool returnTicket(const std::string& uid);
	std::string checkTicket(const std::string& uid);
	std::string checkUser(const std::string& username);
};

#endif /* LOGICPROCESSOR_H_ */
