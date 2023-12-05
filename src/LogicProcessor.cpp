/*
 * LogicProcessor.cpp
 *
 *  Created on: Nov 23, 2023
 *      Author: artem
 */

#include "LogicProcessor.h"

LogicProcessor::LogicProcessor(ConfigReader& conf)
{
	planeRoutesBufferPtr = conf.getPlaneBuffer();
}


std::vector<std::pair<char, int>> LogicProcessor::ListFreePlaces(const std::string& date, const std::string& flightnum) {
	std::vector<Airplane> planeRoutesBuffer = *planeRoutesBufferPtr;

	for (int i = 0; i < planeRoutesBuffer.size(); i++)
	{
		if (planeRoutesBuffer[i].Date == date && planeRoutesBuffer[i].FlightNumber == flightnum)
		{
			planeRoutesBuffer[i].make_all_tickets_map();
			std::vector<std::pair<char, int>> res;

			for (int m = 0; m < planeRoutesBuffer[i].seats.size(); m++)
			{
				if (planeRoutesBuffer[i].tickets.size() == 0)
				{
					res.push_back(planeRoutesBuffer[i].seats[m]);
				}
				else
				{
					for (int k = 0; k < planeRoutesBuffer[i].tickets.size(); k++)
					{
						std::pair<char, int> xxx = std::make_pair(char(), int());
						xxx.first = planeRoutesBuffer[i].tickets[k].SeatLiteral;
						xxx.second = planeRoutesBuffer[i].tickets[k].SeatNumber;
						if (planeRoutesBuffer[i].seats[m] != xxx)
						{
							res.push_back(planeRoutesBuffer[i].seats[m]);
						}
					}
				}
			}

			return res;
		}
	}

	return {};
}

bool LogicProcessor::checkFreePlace(const std::string& date, const std::string& flightnum, const char& place, const int& seatnum) {
	std::vector<Airplane> planeRoutesBuffer = *planeRoutesBufferPtr;

	for (int i = 0; i < planeRoutesBuffer.size(); i++)
	{
		if (planeRoutesBuffer[i].Date == date && planeRoutesBuffer[i].FlightNumber == flightnum)
		{
			for (int j = 0; j < planeRoutesBuffer[i].tickets.size(); j++)
			{
				if (planeRoutesBuffer[i].tickets[j].SeatLiteral == place && planeRoutesBuffer[i].tickets[j].SeatNumber == seatnum)
				{
					return 0;
				}
			}
		}
	}

	return 1;
}

std::string LogicProcessor::addTicket(const std::string &date, const std::string &flightnum, const Ticket &tick) {
	for (auto& it : *planeRoutesBufferPtr)
	{
		if (it.Date == date && it.FlightNumber == flightnum)
		{
			it.tickets.push_back(tick);
		}
	}

	return tick.uid;

}

bool LogicProcessor::returnTicket(const std::string& uid) {
    std::vector<Airplane>& planeRoutesBuffer = *planeRoutesBufferPtr;

    for (auto& airplane : planeRoutesBuffer)
    {
        for (auto it = airplane.tickets.begin(); it != airplane.tickets.end(); ++it)
        {
            if (it->uid == uid)
            {
                airplane.tickets.erase(it);
                return true;
            }
        }
    }

    return false;
}

std::string LogicProcessor::checkTicket(const std::string &uid) {
	std::vector<Airplane>& planeRoutesBuffer = *planeRoutesBufferPtr;

	for (auto& airplane : planeRoutesBuffer)
	{
		for (auto it = airplane.tickets.begin(); it != airplane.tickets.end(); ++it)
		{
			if (it->uid == uid)
			{
				std::string s = "";
				int seatNumber = it->SeatNumber;

				for (const auto& range : airplane.SeatPriceBuffer)
				{
					int start = std::stoi(range.first.substr(0, range.first.find('-')));
					int end = std::stoi(range.first.substr(range.first.find('-') + 1));

					if (seatNumber >= start && seatNumber <= end)
					{
						s += "Flight " + airplane.FlightNumber + " " + airplane.Date + " seat " + std::string(1, it->SeatLiteral) + " " + std::to_string(seatNumber) + " username: " + it->username;
						s += "\nPrice: " + std::to_string(range.second) + "$\n\n";
						return s;
					}
				}
			}
		}
	}

	return "not found";
}

std::string LogicProcessor::checkUser(const std::string &username) {
	std::vector<Airplane>& planeRoutesBuffer = *planeRoutesBufferPtr;
	std::string s = "";

	for (auto& airplane : planeRoutesBuffer)
	{
		for (auto it = airplane.tickets.begin(); it != airplane.tickets.end(); ++it)
		{
			if (it->username == username)
			{
				int seatNumber = it->SeatNumber;

				for (const auto& range : airplane.SeatPriceBuffer) {
					int start = std::stoi(range.first.substr(0, range.first.find('-')));
					int end = std::stoi(range.first.substr(range.first.find('-') + 1));

					if (seatNumber >= start && seatNumber <= end) {
						s += "Flight " + airplane.FlightNumber + " " + airplane.Date + " seat " + std::string(1, it->SeatLiteral) + " " + std::to_string(seatNumber) + " username: " + it->username;
						s += "\nPrice: " + std::to_string(range.second) + "$\n\n";
						break;
					}
				}
			}
		}
	}

	return s;
}
