#include "../../utils.hh"

int main (int argc, char** argv) {
	int sum_span = 25;
	if (1 < argc)
		sum_span = std::atoi(argv[1]);

	std::vector<u64> values;
	values.reserve(1000);
	std::string line;
	while (true) {
		std::getline(std::cin, line);
		if (line.empty())
			break;
		values.push_back(std::stoull(line));
	}

	int fault_index = 0;

	for (int i = sum_span; i < values.size(); ++i) {
		for (int a = i - sum_span; a < i; ++a)
			for (int b = a + 1; b < i; ++b)
				if (values[i] == values[a] + values[b])
					goto search_continue;
		fault_index = i;
		std::cout << values[i] << std::endl;
		break;
	search_continue:
		;
	}

	int range_inf = 0;
	int range_sup = 1;
	u64 range_sum = values[0];
	while (true) {
		if (range_sum == values[fault_index] && 2 < range_sup - range_inf)
			break;
		if (range_sum < values[fault_index]) {
			if (values.size() <= range_sup)
				break;
			range_sum += values[range_sup];
			++range_sup;
		} else {
			range_sum -= values[range_inf];
			++range_inf;
		}
	}

	// std::cerr << "range inf = " << range_inf << " -> " << values[range_inf] << std::endl;
	// std::cerr << "range sup = " << (range_sup-1) << " -> " << values[range_sup-1] << std::endl;

	auto it_inf = values.begin() + range_inf;
	auto it_sup = values.begin() + range_sup;
	u64 range_min = *std::min_element(it_inf, it_sup);
	u64 range_max = *std::max_element(it_inf, it_sup);
	// std::cerr << "min = " << range_min << std::endl;
	// std::cerr << "max = " << range_max << std::endl;

	u64 weakness = range_min + range_max;
	std::cout << weakness << std::endl;

	return 0;
}
