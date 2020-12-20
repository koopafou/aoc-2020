#include "../../utils.hh"

struct Vec {
	int x;
	int y;
};

const Vec directions[] =
	{
		Vec{-1, -1}, Vec{0, -1}, Vec{1, -1}, Vec{1, 0},
		Vec{1, 1}, Vec{0, 1}, Vec{-1, 1}, Vec{-1, 0}
	};

struct Automaton {
	std::vector<std::string> seats;
	std::vector<std::string> newseats;

	Automaton (const std::vector<std::string>& init_seats)
		: seats(init_seats)
		, newseats(init_seats)
	{}

	u32 run (bool ranged, bool log) {
		while (true) {
			if (log) {
				for (auto& line : seats)
					std::cerr << line << std::endl;
				std::cerr << std::endl;
			}
			bool changed = false;
			for (int i = 0; i < seats.size(); ++i)
				for (int j = 0; j < seats[i].size(); ++j) {
					if (seats[i][j] == '.')
						continue;
					u32 occupied_count = 0;
					for (Vec dir : directions) {
						int dist = 1;
						do {
							int x = i + dist * dir.x;
							int y = j + dist * dir.y;
							if (x < 0 || seats.size() <= x ||
							    y < 0 || seats[x].size() <= y)
								break;
							if (seats[x][y] == '.')
								++dist;
							else {
								occupied_count += (seats[x][y] == '#');
								break;
							}
						} while (ranged);
					}
					if (seats[i][j] == '#')
						newseats[i][j] = ((ranged ? 5 : 4) <= occupied_count ? 'L' : '#');
					if (seats[i][j] == 'L')
						newseats[i][j] = (occupied_count ? 'L' : '#');
					changed |= (seats[i][j] != newseats[i][j]);
				}
			if (!changed)
				break;
			std::swap(seats, newseats);
		}

		u32 occupied = 0;
		for (int i = 0; i < seats.size(); ++i)
			for (int j = 0; j < seats[i].size(); ++j)
				if (seats[i][j] == '#')
					++occupied;
		return occupied;
	}
};

int main () {
	std::vector<std::string> init_seats;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		init_seats.emplace_back(std::move(line));
	}

	Automaton at1(init_seats);
	std::cout << at1.run(false, false) << std::endl;

	Automaton at2(init_seats);
	std::cout << at2.run(true, false) << std::endl;

	return 0;
}
