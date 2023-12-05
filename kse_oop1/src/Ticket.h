/*
 * Ticket.h
 *
 *  Created on: Nov 23, 2023
 *      Author: artem
 */

#ifndef TICKET_H_
#define TICKET_H_

#include "includes.h"

class Ticket {
public:
	Ticket();

	Ticket(const char& lit, const int& seatnum, const std::string& usrname);

	std::string uid = "test";
	std::string username;
	char SeatLiteral;
	int SeatNumber = 0;
};

#endif /* TICKET_H_ */
