#include "../../utils.hh"

int main () {
	std::vector<u32> seats;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.size() == 0)
			break;
		u32 value = 0;
		for (char c : line) {
			value *= 2;
			if (c == 'B' || c == 'R')
				++value;
		}
		seats.push_back(value);
	}

	std::sort(seats.begin(), seats.end());

	std::cout << seats.back() << std::endl;

	for (u32 i = 0; i < seats.back() - 1; ++i) {
		if (seats[i] + 2 == seats[i + 1]) {
			std::cout << (seats[i] + 1) << std::endl;
			break;
		}
	}

	return 0;
}
