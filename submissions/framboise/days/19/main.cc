#include <regex>
#include "../../utils.hh"
using u16 = uint16_t;

struct RuleAtom {
	u16 subrule;
	char c;
};

std::regex regrule("([0-9]+):(.*)"); // rule index, alts
std::regex regalt("([^|]*)(?:\\|(.*))?"); // seq, more alts
std::regex regseq("[ ]*(?:(?:\"([a-z])\")|([0-9]+))[ ]*(.*)[ ]*"); // (char | index), more seqs

using RuleSeq = std::vector<RuleAtom>;
using RuleAlt = std::vector<RuleSeq>;
struct Ruleset {
	std::vector<RuleAlt> rules;

	void add (const std::string& line) {
		std::smatch sma;
		if (!std::regex_match(line, sma, regrule))
			throw std::runtime_error("rule match failed");
		u16 rdex = std::stoi(sma[1].str());
		if (rules.size() <= rdex)
			rules.resize(rdex + 1);
		auto& ralt = rules[rdex];
		ralt.clear();
		std::string stralts = sma[2].str();
		while (stralts.size()) {
			if (!std::regex_match(stralts, sma, regalt))
				throw std::runtime_error("alt match failed");
			RuleSeq rseq;
			std::string new_stralts = sma[2];
			std::string strseq = sma[1].str();
			while (strseq.size()) {
				if (!std::regex_match(strseq, sma, regseq))
					throw std::runtime_error("seq match failed");
				if (sma[1].matched)
					rseq.push_back(RuleAtom{UINT16_MAX, sma[1].str()[0]});
				else
					rseq.push_back(RuleAtom{std::stoi(sma[2].str()), 0});
				strseq = sma[3].str();
			}
			ralt.emplace_back(std::move(rseq));
			stralts = std::move(new_stralts);
		}
	}

	// don't even need to optimize by selecting preferential alts depending on the next char to read ?
	using It = std::string::const_iterator;
	bool word_match_impl (It begin, It end, u16 r, It& match_term) {
		for (auto& seq : rules[r]) {
			It sub_mt = begin;
			bool match = true;
			for (auto& atom : seq) {
				if (atom.subrule == UINT16_MAX) {
					if (sub_mt != end && atom.c == *sub_mt)
						++sub_mt;
					else
						match = false;
				} else {
					match = word_match_impl(sub_mt, end, atom.subrule, sub_mt);
				}
				if (!match)
					break;
			}
			if (match) {
				match_term = sub_mt;
				return true;
			}
		}
		return false;
	}

	bool word_match (const std::string& str) {
		It end;
		return word_match_impl(str.begin(), str.end(), 0, end) && end == str.end();
	}
};

int main () {
	Ruleset rs;
	rs.rules.reserve(150);
	std::vector<std::string> words;
	words.reserve(1000);
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		rs.add(line);
	}
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		words.emplace_back(std::move(line));
	}

	auto count_matches = [&] () {
		u32 count_match = 0;
		for (auto& word : words) {
			bool match = rs.word_match(word);
			std::cerr << match << " >> " << word << std::endl;
			count_match += match;
		}
		std::cerr << "---" << std::endl;
		std::cout << count_match << std::endl;
	};

	count_matches();
	rs.add("8: 42 | 42 8");
	rs.add("11: 42 31 | 42 11 31");

#if 0
	for (u16 i = 0; i < rs.rules.size(); ++i) {
		auto& alt = rs.rules[i];
		std::cerr << i << ": ";
		bool first_seq = true;
		for (auto& seq : alt) {
			if (!first_seq)
				std::cerr << " | ";
			first_seq = false;
			bool first_atom = true;
			for (auto& atom : seq) {
				if (!first_atom)
					std::cerr << " ";
				first_atom = false;
				if (atom.subrule == UINT16_MAX)
					std::cerr << '"' << atom.c << '"';
				else
					std::cerr << atom.subrule;
			}
		}
		std::cerr << std::endl;
	}
#endif

	count_matches();

	return 0;
}
