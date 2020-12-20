#include <unordered_map>
#include "../../utils.hh"

int main () {
	std::vector<i32> start;
	while (true) {
		std::string line;
		std::getline(std::cin, line, ',');
		if (line.empty())
			break;
		start.push_back(std::stoi(line));
	}

	std::unordered_map<i32, std::pair<i32, i32>> track;
	i32 last_spoken = -1;
	i32 count_spoken = 0;
	auto speak = [&] (i32 v) {
		last_spoken = v;
		auto it = track.find(v);
		if (it == track.end())
			track[v] = std::make_pair(-1, count_spoken);
		else {
			it->second.first = it->second.second;
			it->second.second = count_spoken;
		}
		++count_spoken;
	};
	for (auto v : start)
		speak(v);
	while (count_spoken < 30000000) {
		if (count_spoken == 2020)
			std::cout << last_spoken << std::endl;
		auto it = track.find(last_spoken);
		if (it == track.end() || it->second.first == -1)
			speak(0);
		else
			speak(it->second.second - it->second.first);
	}

	std::cout << last_spoken << std::endl;

	return 0;
}
