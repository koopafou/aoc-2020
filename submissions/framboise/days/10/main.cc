#include "../../utils.hh"

struct Head {
	u64 combo;
	u32 last_value;
};

int main () {
	std::vector<u32> adapters;
	adapters.reserve(1000);
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		adapters.push_back(std::stoi(line));
	}

	std::sort(adapters.begin(), adapters.end());

	u32 diffcnt[4] = {0, 0, 0, 1};
	if (3 < adapters[0])
		return 1;
	++diffcnt[adapters[0]];
	for (int i = 0; i < adapters.size() - 1; ++i) {
		u32 diff = adapters[i + 1] - adapters[i];
		if (3 < diff)
			return 1;
		++diffcnt[diff];
	}
	std::cout << (diffcnt[1] * diffcnt[3]) << std::endl;

	if (diffcnt[0]) {
		std::cerr << "duplicates ?" << std::endl;
		return 1;
	}

	std::vector<Head> heads;
	heads.push_back(Head{1, 0});

	for (int i = 0; i < adapters.size(); ++i) {
		u64 combo_take = 0;
		std::vector<Head> newheads;
		newheads.reserve(heads.size() + 1);
		for (auto& hd : heads) {
			if (adapters[i] <= hd.last_value + 3) {
				combo_take += hd.combo;
				newheads.push_back(hd);
			}
		}
		if (combo_take)
			newheads.push_back(Head{combo_take, adapters[i]});
		std::swap(heads, newheads);
	}

	// last adapter must always "take" because the diff with the destination is 3
	std::cout << heads.back().combo << std::endl;

	return 0;
}
