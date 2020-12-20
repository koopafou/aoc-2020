#include <unordered_map>
#include <regex>
#include "../../utils.hh"

struct Mask {
	u64 set;
	u64 unset;
};

struct Assign {
	u64 index;
	u64 value;
};

struct Command {
	bool is_mask;
	union { Mask mask; Assign assign; };
};

std::unordered_map<u64, u64> xmemory;
u64 xmemsum = 0;

void memwrite (u64 floating, u64 bitdex, u64 index, u64 value) {
	if (floating & 1) {
		memwrite(floating >> 1, bitdex + 1, index & ~((u64)1 << bitdex), value);
		memwrite(floating >> 1, bitdex + 1, index |  ((u64)1 << bitdex), value);
	} else if (floating) {
		memwrite(floating >> 1, bitdex + 1, index, value);
	} else {
		u64& memcell = xmemory[index];
		xmemsum -= memcell;
		memcell = value;
		xmemsum += memcell;
	}
}

int main () {
	std::vector<Command> commands;
	commands.reserve(1000);
	std::regex reg("(?:mask = ([01X]+))|(?:mem\\[([0-9]+)\\] = ([0-9]+))");
	std::smatch sma;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (!std::regex_match(line, sma, reg))
			break;
		auto match_mask = sma[1];
		auto match_memdex = sma[2];
		auto match_memval = sma[3];
		Command cmd;
		if (match_mask.matched) {
			cmd.is_mask = true;
			cmd.mask.set = 0;
			cmd.mask.unset = 0;
			for (char c : match_mask.str()) {
				cmd.mask.set *= 2;
				cmd.mask.unset *= 2;
				if (c == '0')
					++cmd.mask.unset;
				if (c == '1')
					++cmd.mask.set;
			}
		} else {
			cmd.is_mask = false;
			cmd.assign.index = std::stoull(match_memdex.str());
			cmd.assign.value = std::stoull(match_memval.str());
		}
		commands.push_back(cmd);
	}

	std::unordered_map<u64, u64> memory;
	u64 memsum = 0;
	Mask default_mask{0,0};
	Mask* current_mask = &default_mask;
	for (auto& cmd : commands) {
		if (cmd.is_mask) {
			current_mask = &cmd.mask;
			continue;
		}
		u64& memcell = memory[cmd.assign.index];
		memsum -= memcell;
		memcell = (cmd.assign.value & ~current_mask->unset) | current_mask->set;
		memsum += memcell;
	}
	std::cout << memsum << std::endl;

	// ooga booga actually works ?
	current_mask = &default_mask;
	u64 floating = 0;
	for (auto& cmd : commands) {
		if (cmd.is_mask) {
			current_mask = &cmd.mask;
			floating = ~(current_mask->set | current_mask->unset) & (((u64)1 << 36) - 1);
			continue;
		}
		memwrite(floating, 0, cmd.assign.index | current_mask->set, cmd.assign.value);
	}
	std::cout << xmemsum << std::endl;

	return 0;
}
