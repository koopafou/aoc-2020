#include "../../utils.hh"

std::ostream& operator<< (std::ostream& o, __int128 x) {
	std::string str(40, ' ');
	bool neg = false;
	if (x < 0) {
		neg = true;
		x = -x;
	}
	int dex = str.size() - 1;
	while (x) {
		str[dex] = '0' + x % 10;
		x /= 10;
		--dex;
	}
	if (neg) {
		str[dex] = '-';
		--dex;
	}
	return o << str.c_str() + dex + 1;
}

struct EedState {
	__int128 r, s, t;
};

__int128 correct_mod (__int128 a, __int128 b) {
	return ((a%b)+b)%b;
}

// extended euclidean division
// return {r,s,t} such that gcd(a, b) == r == a * s + b * t
EedState eed (__int128 a, __int128 b) {
	EedState state_[] = {
		{a, 1, 0},
		{b, 0, 1},
		{0, 0, 0},
		{0, 0, 0}
	};
	u32 dex = 0;
	auto state = [&] (int offset) -> EedState& {
		return state_[(dex + offset) & 3];
	};
	while (state(1).r != 0) {
		__int128 q = state(0).r / state(1).r;
		state(2).r = state(0).r - q * state(1).r;
		state(2).s = state(0).s - q * state(1).s;
		state(2).t = state(0).t - q * state(1).t;
		++dex;
	}
	std::cerr << "eed>> " << a << " * " << state(0).s << " + " << b << " * " << state(0).t << std::endl;
	return state(0);
}

struct CrtEntry {
	__int128 index;
	__int128 cst;
	__int128 mod;
};

// chinese remainder theorem
// return x such that for each i, x % system[i].mod == system[i].cst
// assume 0 <= cst < mod
__int128 crt (const std::vector<CrtEntry>& system) {
	CrtEntry master = system[0];
	for (int i = 1; i < system.size(); ++i) {
		EedState state = eed(master.mod, system[i].mod);
		if (state.r != 1)
			throw std::runtime_error("modules aren't coprimes");
		// that shit can overflow smh
		//master.cst = master.cst * system[i].mod * state.t + system[i].cst * master.mod * state.s;
		// well actually it still overflows 64 bits
		master.cst = system[i].cst + (master.cst - system[i].cst) * system[i].mod * state.t;
		master.mod *= system[i].mod;
		std::cerr << " . >>> " << master.cst << std::endl;
		master.cst = correct_mod(master.cst, master.mod);
		std::cerr << "cst>>> " << master.cst << std::endl;
		std::cerr << "mod>>> " << master.mod << std::endl;
		for (int j = 0; j <= i; ++j) {
			__int128 result = ((master.cst + system[j].index) % system[j].mod);
			if (result)
				std::cerr << "equation #" << j << " not fullfilled : " << result << std::endl;
		}
	}
	return master.cst;
}

int main () {
	i64 start_stamp;
	std::vector<__int128> ids;
	(std::cin >> start_stamp).ignore();
	while (true) {
		std::string line;
		std::getline(std::cin, line, ',');
		if (line.empty())
			break;
		if (line[0] == 'x')
			ids.push_back(1);
		else
			ids.push_back(std::stoi(line));
	}

	__int128 min_dex = 0;
	__int128 min_depart = INT64_MAX;
	for (__int128 i = 0; i < ids.size(); ++i) {
		if (ids[i] == 1)
			continue;
		__int128 departure = (start_stamp / ids[i] + 1) * ids[i];
		if (departure < min_depart) {
			min_depart = departure;
			min_dex = i;
		}
	}
	std::cout << (ids[min_dex] * (min_depart - start_stamp)) << std::endl;

	std::vector<CrtEntry> system;
	system.reserve(ids.size());
	for (__int128 i = 0; i < ids.size(); ++i) {
		if (ids[i] != 1) {
			__int128 cst = correct_mod(-i, ids[i]);
			system.push_back(CrtEntry{i, cst, ids[i]});
			std::cerr << "solve>> x === " << cst << " mod " << ids[i] << std::endl;
		}
	}
	__int128 x = crt(system);
	std::cout << x << std::endl;

	return 0;
}
