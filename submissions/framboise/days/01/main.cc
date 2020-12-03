#include "../../utils.hh"

constexpr u32 sum = 2020;

int main () {
	std::vector<u32> inputs;
	inputs.reserve(1000);
	while (std::cin) {
		u32 in;
		(std::cin >> in).ignore();
		inputs.push_back(in);
	}

	// let n = inputs.size()

	std::sort(inputs.begin(), inputs.end()); // O(n * log(n))

	// let y = d inputs / dn, the array of differences between each input after sorting
	// let m = max(y) / min(y)
	// m is expected to have a very small effect on complexity anyway

	auto search = [&] (i32 input_k, i32 factor_k, i32 skip_k) {
		i32 i = skip_k + 1;
		i32 j = inputs.size() - 1;
		while (i < inputs.size()) {
			while (0 < j && sum < inputs[i] + inputs[j] + input_k) {
				--j;
			}
			while (j < inputs.size() && inputs[i] + inputs[j] + input_k < sum) {
				++j;
			}
			if (inputs[i] + inputs[j] + input_k == sum && skip_k < j && j < i) {
				std::cout << std::format("{}\n", inputs[i] * inputs[j] * factor_k);
			}
			++i;
		}
	};

	// STAR 1
	// O(n * m) ?
	search(0, 1, -1);

	// STAR 2
	// O(n^2 * m) ?
	for (i32 k = 0; k < inputs.size(); ++k)
		search(inputs[k], inputs[k], k);

	return 0;
}
