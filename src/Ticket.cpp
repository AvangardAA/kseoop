/*
 * Ticket.cpp
 *
 *  Created on: Nov 23, 2023
 *      Author: artem
 */

#include "Ticket.h"

Ticket::Ticket() {
	// TODO Auto-generated constructor stub

}

Ticket::Ticket(const char& lit, const int& seatnum, const std::string& usrname)
{
	username = usrname;
	SeatLiteral = lit;
	SeatNumber = seatnum;

	std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, charset.size() - 1);

	std::string gUid;
	for (int i = 0; i < 10; ++i) {
		gUid += charset[distr(gen)];
	}

	uid = gUid;
}
