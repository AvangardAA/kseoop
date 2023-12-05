//============================================================================
// Name        : kse_oop.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "includes.h"
#include "ConfigReader.h"
#include "Airplane.h"
#include "LogicProcessor.h"

int main() {
	ConfigReader conf{"config.txt"};

	std::vector<Airplane> debugBuf = *(conf.getPlaneBuffer());

	LogicProcessor logic_e{conf};
	std::string opt = "";

	while (true)
	{
		std::cout << "\n\nchoose option: ";
		std::cin >> opt;

		if (opt == "check")
		{
			std::string date = "";
			std::string flightnum = "";

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << "Enter date and flight number: ";
			std::cin >> date >> flightnum;

			std::vector<std::pair<char, int>> res = logic_e.ListFreePlaces(date, flightnum);

			if (!res.empty())
			{
				std::cout << "Free seats below" << std::endl;

				for (int i = 0; i < res.size(); i++)
				{
					std::cout << res[i].first << " " << res[i].second;
				}
			}
			else std::cout << "No free spaces" << std::endl;
		}

		else if (opt == "book")
		{
			std::string date = "";
			std::string flightnum = "";
			std::string place = "";
			std::string usrname = "";

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << "Enter date and flight number: ";
			std::cin >> date >> flightnum;
			std::cout << "\nInput place and username: ";
			std::cin >> place >> usrname;

			char lit = 'A';

			lit = place.back();
			place.pop_back();

			auto fc = [&place]()->int{
				return std::stoi(place);
			};

			if (!logic_e.checkFreePlace(date, flightnum, lit, fc())) std::cout << " already occupied place" << std::endl;
			else std::cout << logic_e.addTicket(date, flightnum, Ticket(lit, fc(), usrname));
		}
		else if (opt == "return")
		{
			std::string uid = "";

			std::cout << "enter ticket uid: ";
			std::cin >> uid;

			if (logic_e.returnTicket(uid))
			{
				std::cout << "returned successfuly" << std::endl;
			}
			else std::cout << "ticket not found" << std::endl;
		}
		else if (opt == "view")
		{
			std::string optinternal = "";
			std::string uid = "";

			std::cin >> optinternal;

			if (optinternal == "uid")
			{
				std::cout << "enter: ";
				std::cin >> uid;

				std::cout << logic_e.checkTicket(uid);
			}
			else
			{
				std::cin >> uid;
				std::cout << logic_e.checkUser(uid);
			}
		}
	}


	return 0;
}
