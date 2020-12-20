#include <glm/glm.hpp>
#include <glm/gtc/matrix_integer.hpp>
#include "../../utils.hh"

using ivec = glm::tvec2<i32>;
using imat = glm::tmat2x2<i32>;

struct Command {
	char cmd;
	i32 dist;
};

std::ostream& operator<< (std::ostream& o, const ivec& v) {
	return o << "{" << v.x << ',' << v.y << "}";
}

int main () {
	std::vector<Command> cmds;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		cmds.push_back(Command{line[0], std::atoi(line.c_str() + 1)});
	}

	ivec pos{0,0};
	ivec dir{1,0};

	// matrices are fucking column-major

	imat lturn
		{0, 1,
		 -1, 0};

	imat rturn
		{0, -1,
		 1, 0};

	for (auto& command : cmds) {
		switch (command.cmd) {
		case 'N': pos.y += command.dist; break;
		case 'S': pos.y -= command.dist; break;
		case 'E': pos.x += command.dist; break;
		case 'W': pos.x -= command.dist; break;
		case 'L': {
			i32 angle = command.dist;
			if (angle % 90 != 0)
				return 1;
			while (angle) {
				dir = lturn * dir;
				angle -= 90;
			}
		} break;
		case 'R': {
			i32 angle = command.dist;
			if (angle % 90 != 0)
				return 1;
			while (angle) {
				dir = rturn * dir;
				angle -= 90;
			}
		} break;
		case 'F': pos += command.dist * dir; break;
		default: return 1;
		}
		//std::cerr << pos << "  //  " << dir << std::endl;
	}
	std::cout << (std::abs(pos.x) + std::abs(pos.y)) << std::endl;

	ivec boatp{0,0};
	ivec wayp{10,1};

	for (auto& command : cmds) {
		switch (command.cmd) {
		case 'N': wayp.y += command.dist; break;
		case 'S': wayp.y -= command.dist; break;
		case 'E': wayp.x += command.dist; break;
		case 'W': wayp.x -= command.dist; break;
		case 'L': {
			i32 angle = command.dist;
			if (angle % 90 != 0)
				return 1;
			while (angle) {
				wayp = lturn * wayp;
				angle -= 90;
			}
		} break;
		case 'R': {
			i32 angle = command.dist;
			if (angle % 90 != 0)
				return 1;
			while (angle) {
				wayp = rturn * wayp;
				angle -= 90;
			}
		} break;
		case 'F': boatp += command.dist * wayp; break;
		default: return 1;
		}
	}
	std::cout << (std::abs(boatp.x) + std::abs(boatp.y)) << std::endl;

	return 0;
}
