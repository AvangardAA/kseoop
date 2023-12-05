/*
 * Airplane.h
 *
 *  Created on: Nov 23, 2023
 *      Author: artem
 */

#ifndef AIRPLANE_H_
#define AIRPLANE_H_

#include "includes.h"
#include "Ticket.h"

class Airplane {
public:
	Airplane(const std::string& buffer);

	std::string Date = "";
	std::string FlightNumber = "";
	int sitNumber = 0;
	std::vector<std::pair<std::string, int>> SeatPriceBuffer;
	std::vector<std::pair<char, int>> seats;
	std::vector<Ticket> tickets = {};

	void make_all_tickets_map();
};

#endif /* AIRPLANE_H_ */
