#include <glm/glm.hpp>
#include <glm/gtx/component_wise.hpp>
#include "../../utils.hh"

using ivec = glm::tvec4<i32>;

template <u32 DIM, u32 K, typename F>
void foreach_block_impl (i32 margin, ivec span, const F& f, ivec& pback) {
	if constexpr (K == UINT32_MAX)
		f(pback);
	else
		for (pback[K] = margin; pback[K] < span[K] - margin; ++pback[K])
			foreach_block_impl<DIM, K - 1>(margin, span, f, pback);
}

template <u32 DIM, typename F>
void foreach_block (i32 margin, ivec span, const F& f) {
	ivec pback(0);
	foreach_block_impl<DIM, DIM - 1>(margin, span, f, pback);
}

template <u32 DIM>
struct Map {
	ivec span;
	std::vector<char> map;

	i32 dex (ivec p) const {
		static_assert(DIM <= 4);
		i32 dex = p[DIM - 1];
		for (int i = DIM - 2; 0 <= i; --i)
			dex = (dex * span[i] + p[i]);
		return dex;
	}

	char operator() (ivec p) const {
		return map[dex(p)];
	}

	char& operator() (ivec p) {
		return map[dex(p)];
	}

	template <typename F>
	void foreach (i32 margin, const F& f) const {
		foreach_block<DIM>(margin, span, f);
	}

	void init (const std::vector<std::string>& slice) {
		span.x = slice[0].size() + 4;
		span.y = slice.size() + 4;
		for (int i = 2; i < DIM; ++i)
			span[i] = 1 + 4;
		for (int i = DIM; i < 4; ++i)
			span[i] = 1;
		map = std::vector<char>(glm::compMul(span), '.');
		foreach(2, [&] (ivec p) {
			(*this)(p) = slice[p.y-2][p.x-2];
		});
	}

	void simround (const Map& prev) {
		ivec min(prev.span);
		ivec max(0);
		prev.foreach(0, [&] (ivec p) {
			if (prev(p) == '.')
				return;
			min = glm::min(min, p);
			max = glm::max(max, p);
		});
		if (max.x < min.x)
			throw std::runtime_error("no active cell");
		span = max + ivec(1) - min + ivec(6);
		for (int i = DIM; i < 4; ++i)
			span[i] = 1;
		map.resize(glm::compMul(span));
		for (char& c : map)
			c = '.';

		ivec off = min - ivec(3);
		foreach(2, [&] (ivec p) {
			i32 active_count = 0;
			foreach_block<DIM>(-1, ivec(1), [&] (ivec q) {
				active_count += (prev(off + p + q) == '#');
			});
			bool active_current = (prev(off + p) == '#');
			active_count -= active_current;
			(*this)(p) = (active_current ?
			              (active_count == 2 || active_count == 3 ? '#' : '.') :
			              (active_count == 3 ? '#' : '.'));
		});
	}
};

template <u32 DIM>
std::ostream& operator<< (std::ostream& o, const Map<DIM>& map) {
	static std::string seps[] = {"\n", "\n++++\n", "\n========\n"};
	map.foreach(2, [&] (ivec p) {
		o << map(p);
		if (p.x == map.span.x - 3)
			for (int sepdex = 1; sepdex < DIM; ++sepdex)
				if (p[sepdex] != map.span[sepdex] - 3) {
					o << seps[sepdex - 1];
					return;
				}
	});
	return o << "\n////////////\n";
}

template <u32 DIM>
void run_sim (const std::vector<std::string>& lines) {
	Map<DIM> maps[2];
	maps[0].init(lines);
	int front = 0;
	//std::cerr << maps[0] << std::endl;
	for (int round = 0; round < 6; ++round) {
		maps[1 - front].simround(maps[front]);
		front = 1 - front;
		//std::cerr << maps[front] << std::endl;
	}
	u32 active_count = 0;
	maps[front].foreach(0, [&] (ivec p) {
		active_count += (maps[front](p) == '#');
	});
	std::cout << active_count << std::endl;
}

int main () {
	std::vector<std::string> lines;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		if (lines.size() && lines[0].size() != line.size())
			return 1;
		lines.emplace_back(std::move(line));
	}

	run_sim<3>(lines);
	run_sim<4>(lines);

	return 0;
}
