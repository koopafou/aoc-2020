#include <glm/glm.hpp>
#include <glm/gtc/matrix_integer.hpp>
#include <regex>
#include <array>
#include "../../utils.hh"

using ivec = glm::tvec3<i32>;
using imat = glm::tmat3x3<i32>;

const imat base_rotate = {
	{0,-1, 0},
	{1, 0, 0},
	{0, 9, 1}
};

const imat base_flip = {
	{0, 1, 0},
	{1, 0, 0},
	{0, 0, 1}
};

/* Matching two pieces is equivalent to matching their first row after some transformations.
   There are 8x8/2 possible combinations.
   When we want to reposition them in a fixed frame, we can consider to put them on a checkerboard,
   pick a reference tile, flip all tiles of one color (the flip here comes from how we match the rows
   in order, instead of reversing one of them), and rotate/place tiles depending on their adjacency.
 */

const std::vector<imat> trs = {
	imat(1),
	base_rotate,
	base_rotate * base_rotate,
	base_rotate * base_rotate * base_rotate,
	base_flip,
	base_flip * base_rotate,
	base_flip * base_rotate * base_rotate,
	base_flip * base_rotate * base_rotate * base_rotate
};

struct Tile {
	u64 id;
	std::array<std::array<char, 10>, 10> data;
};

void list_matchable (const Tile* a, const Tile* b,
                     std::vector<std::pair<imat, imat>>& pair_trs) {
	
}

int main () {
	std::regex regid("Tile ([0-9]+):");
	std::smatch sma;
	std::vector<Tile> tiles;
	tiles.reserve(150);
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (!std::regex_match(line, sma, regid))
			break;
		tiles.push_back(Tile{std::stoull(sma[1])});
		for (int x = 0; x < 10; ++x) {
			for (int y = 0; y < 10; ++y)
				std::cin >> tiles.back().data[x][y];
			std::cin.ignore();
		}
		std::cin.ignore();
	}

	// [A_index, B_index], list of transforms on A to match B
	// [B_index, A_index], ^ same list size, but on B to match A
	std::vector<std::vector<imat>> matches;
	matches.resize(tiles.size() * tiles.size());
	for (int adex = 0; adex < tiles.size(); ++adex) {
		for (int bdex = adex + 1; bdex < tiles.size(); ++bdex) {
			auto& cellAB = matches[adex * tiles.size() + bdex];
			auto& cellBA = matches[bdex * tiles.size() + adex];
			for (auto& tra : trs) {
				for (auto& trb : trs) {
					bool matching = true;
					for (int y = 0; y < 10; ++y) {
						ivec pa = tra * ivec{0, y, 1};
						ivec pb = trb * ivec{0, y, 1};
						matching &= (tiles[adex].data[pa.x][pa.y] == tiles[bdex].data[pb.x][pb.y]);
					}
					if (matching) {
						cellAB.push_back(tra);
						cellBA.push_back(trb);
					}
				}
			}
		}
	}

	u64 result = 1;
	// according to this output, it doesn't even seem like there are locally ambiguous matches 
	for (int adex = 0; adex < tiles.size(); ++adex) {
		u32 row_count = 0;
		for (int bdex = 0; bdex < tiles.size(); ++bdex) {
			//std::cerr << matches[adex * tiles.size() + bdex].size();
			row_count += matches[adex * tiles.size() + bdex].size();
		}
		//std::cerr << std::endl;
		if (row_count == 4) // 2 adjacent tiles * 2 possible face-forward
			result *= tiles[adex].id;
	}
	std::cout << result << std::endl;

	return 0;
}
