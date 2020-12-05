#include <set>
#include <map>
#include "../../utils.hh"

using Passport = std::map<std::string, std::string>;

bool validate_int (const std::string& str, int inf, int sup) {
	for (char c : str)
		if (c < '0' || '9' < c)
			return false;
	int ival = std::stoi(str);
	if (ival < inf || sup < ival)
		return false;
	return true;
}

bool validate_hexcolor (const std::string& str) {
	if (str[0] != '#')
		return false;
	for (int i = 1; i < str.size(); ++i)
		if (!(('0' <= str[i] && str[i] <= '9') ||
		      ('a' <= str[i] && str[i] <= 'f')))
			return false;
	return true;
}

std::set<std::string> ecl_list () {
	static std::set<std::string> list;
	if (list.empty()) {
		list.insert("amb");
		list.insert("blu");
		list.insert("brn");
		list.insert("gry");
		list.insert("grn");
		list.insert("hzl");
		list.insert("oth");
	}
	return list;
}

enum Validation {
	VALID_NOT,
	VALID_KEYS,
	VALID_VALUES,

	VALID_N
};

Validation validate (const Passport& pp) {
	bool ppvalid = true;
	for (std::string key : {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"})//, "cid"
		ppvalid &= pp.contains(key);
	if (!ppvalid)
		return VALID_NOT;

	if (!validate_int(pp.at("byr"), 1920, 2002))
		return VALID_KEYS;
	if (!validate_int(pp.at("iyr"), 2010, 2020))
		return VALID_KEYS;
	if (!validate_int(pp.at("eyr"), 2020, 2030))
		return VALID_KEYS;
	auto& hgt = pp.at("hgt");
	int hgt_udex = hgt.size() - 2;
	auto hgt_value = hgt.substr(0, hgt_udex);
	auto hgt_unit = hgt.substr(hgt_udex);
	if (!((hgt_unit == "cm" && validate_int(hgt_value, 150, 193)) ||
	      (hgt_unit == "in" && validate_int(hgt_value, 59, 76))))
		return VALID_KEYS;
	if (!validate_hexcolor(pp.at("hcl")))
		return VALID_KEYS;
	if (!ecl_list().contains(pp.at("ecl")))
		return VALID_KEYS;
	auto& pid = pp.at("pid");
	if (pid.size() != 9)
		return VALID_KEYS;
	for (char c : pid)
		if (c < '0' || '9' < c)
			return VALID_KEYS;
	return VALID_VALUES;
}

int main () {
	std::vector<Passport> passports;
	passports.reserve(1000);
	Passport current;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.size() == 0) {
			passports.emplace_back(std::move(current));
			current.clear();
			if (!std::cin)
				break;
			continue;
		}
		int k_begin = 0;
		int colon = 0;
		for (int i = 0; i < line.size() + 1; ++i) {
			if (i == line.size() || line[i] == ' ') {
				current[line.substr(k_begin, colon - k_begin)] = line.substr(colon + 1, i - colon - 1);
				k_begin = i + 1;
			} else if (line[i] == ':')
				colon = i;
		}
	}

	u32 valid_k = 0;
	u32 valid_v = 0;
	for (auto& pp : passports) {
		Validation valid = validate(pp);
		valid_k += (VALID_KEYS <= valid);
		valid_v += (VALID_VALUES <= valid);
	}

	std::cout << valid_k << std::endl
	          << valid_v << std::endl;

	return 0;
}
