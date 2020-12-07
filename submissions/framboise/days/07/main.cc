#include <set>
#include <map>
#include <regex>
#include "../../utils.hh"

std::map<std::string, std::map<std::string, u32>> bagsettings;
std::map<std::string, std::set<std::string>> reverses;
std::set<std::string> cangold;
std::map<std::string, u64> bagcache;

void goldcheck (const std::string& color) {
	if (cangold.contains(color))
		return;
	cangold.insert(color);
	for (auto& cancolor : reverses[color])
		goldcheck(cancolor);
}

u64 countbag (const std::string& color) {
	auto it = bagcache.find(color);
	if (it != bagcache.end()) {
		if (it->second == -1)
			throw std::runtime_error("recursive bags");
		return it->second;
	}
	auto& cache = bagcache[color];
	cache = -1;
	u64 value = 0;
	for (auto& [subcolor, count] : bagsettings[color])
		value += count * (1 + countbag(subcolor));
	cache = value;
	return cache;
}

int main () {
	// main bag -> sub bag -> amount
	std::regex regmain("([a-z]+ [a-z]+) bags contain (?:(no other bags\\.)|(.*))");
	std::regex regsub("([0-9]+) ([a-z]+ [a-z]+) bags?[,.]");
	std::smatch sma;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		if (!std::regex_match(line, sma, regmain))
			return 1;
		std::string main_name = sma[1].str();
		std::ssub_match no_bags = sma[2];
		std::ssub_match yes_bags = sma[3];
		auto& mainsetting = bagsettings[main_name];
		if (!mainsetting.empty())
			return 2;
		if (no_bags.matched)
			continue;
		std::sregex_iterator end;
		for (auto subit = std::sregex_iterator(yes_bags.first, yes_bags.second, regsub);
		     subit != end; ++subit) {
			auto subsma = *subit;
			std::string sub_name = subsma[2].str();
			std::string sub_count = subsma[1].str();
			auto& subsetting = mainsetting[sub_name];
			if (subsetting != 0)
				return 3;
			subsetting = std::stoi(sub_count);
		}
	}

	for (auto& [mainbag, mainsetting] : bagsettings)
		for (auto& [subbag, count] : mainsetting)
			reverses[subbag].insert(mainbag);

	goldcheck("shiny gold");
	std::cout << (cangold.size() - 1) << std::endl;

	std::cout << countbag("shiny gold") << std::endl;

	for (auto& [color, weight] : bagcache)
		std::cout << color << " : " << weight << std::endl;

	return 0;
}
