#include <bit>
#include "../../utils.hh"

using Group = std::vector<u32>;

int main () {
	std::vector<Group> groups;
	Group cur_group;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty()) {
			if (cur_group.empty())
				break;
			groups.emplace_back(std::move(cur_group));
			cur_group.clear();
		} else {
			u32 qset = 0;
			for (char c : line)
				qset |= (1 << (c - 'a'));
			cur_group.push_back(qset);
		}
	}

	u32 count1 = 0;
	u32 count2 = 0;
	for (auto& group : groups) {
		u32 gqset1 = 0;
		u32 gqset2 = (u32)-1;
		for (u32 qset : group) {
			gqset1 |= qset;
			gqset2 &= qset;
		}
		count1 += std::popcount(gqset1);
		count2 += std::popcount(gqset2);
	}
	std::cout << count1 << std::endl
	          << count2 << std::endl;

	return 0;
}
