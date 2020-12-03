#include "../../utils.hh"

struct Input {
	u32 inf;
	u32 sup;
	char c;
	std::string pwd;
};

int main () {
	std::vector<Input> inputs;
	inputs.reserve(1000);
	while (true) {
		Input in;
		char pwd[256];
		if (scanf(SU32 "-" SU32 " %c: %s\n", &in.inf, &in.sup, &in.c, pwd) == EOF)
			break;
		in.pwd = pwd;
		inputs.push_back(in);
	}

	u32 valid1 = 0;
	u32 valid2 = 0;
	for (auto& in : inputs) {
		u32 count = 0;
		for (char c : in.pwd)
			if (c == in.c)
				++count;
		if (in.inf <= count && count <= in.sup)
			++valid1;
		if ((in.pwd[in.inf - 1] == in.c) !=
		    (in.pwd[in.sup - 1] == in.c))
			++valid2;
	}
	std::cout << valid1 << std::endl
	          << valid2 << std::endl;

	return 0;
}
