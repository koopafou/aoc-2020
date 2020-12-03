#include "../../utils.hh"

int main () {
	std::vector<std::string> inputs;
	inputs.reserve(400);
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (!std::cin)
			break;
		inputs.emplace_back(std::move(line));
	}
	u32 width = inputs.size();
	u32 length = inputs[0].size();

	auto count = [&] (u32 lstep, u32 wstep) {
		u64 trees = 0;
		for (u32 i = 0; i * wstep < width; ++i) {
			if (inputs[i * wstep][(i * lstep) % length] == '#')
				++trees;
		}
		return trees;
	};

	std::cout << count(3, 1) << std::endl;
	std::cout << (count(1, 1) * count(3, 1) * count(5, 1) * count(7, 1) * count(1, 2)) << std::endl;

	return 0;
}
