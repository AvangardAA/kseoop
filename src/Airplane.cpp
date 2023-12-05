/*
 * Airplane.cpp
 *
 *  Created on: Nov 23, 2023
 *      Author: artem
 */

#include "Airplane.h"

Airplane::Airplane(const std::string& buffer) {
	std::istringstream iss(buffer);
	std::string sub;
	int counter = 0;
	std::pair<std::string, int> seatsPrice = std::make_pair(std::string(), int());
 	while (iss >> sub)
	{
 		switch (counter)
		{
		case 0:
			Date = sub;
			counter++;
			break;
		case 1:
			FlightNumber = sub;
			counter++;
			break;
		case 2:
			sitNumber = std::stoi(sub);
			counter++;
			break;
		default:
			if (seatsPrice.second == 0)
			{
				seatsPrice.first = sub;
				seatsPrice.second = 1;
			}
			else
			{
				auto fc = [&sub]()->int{
					sub.pop_back();
					return std::stoi(sub);
				};
				seatsPrice.second = fc();
				SeatPriceBuffer.push_back(seatsPrice);
				seatsPrice.second = 0;
			}
			break;
		}
	}
}

void Airplane::make_all_tickets_map() {
	std::vector<char> chars = {'A', 'B', 'C', 'D', 'E', 'F'};

	int totalSeats = 0;
	for (const auto& range : SeatPriceBuffer) {
		int start = std::stoi(range.first.substr(0, range.first.find('-')));
		int end = std::stoi(range.first.substr(range.first.find('-') + 1));
		totalSeats += (end - start + 1);
	}

	int seatsPerChar = totalSeats / chars.size();

	int charIndex = 0;
	for (const auto& range : SeatPriceBuffer) {
		int start = std::stoi(range.first.substr(0, range.first.find('-')));
		int end = std::stoi(range.first.substr(range.first.find('-') + 1));

		for (int i = 0; i < seatsPerChar; ++i) {
			for (int j = start; j <= end; ++j) {
				seats.push_back(std::make_pair(chars[charIndex], j));
			}
			charIndex = (charIndex + 1) % chars.size();
		}
	}
}
