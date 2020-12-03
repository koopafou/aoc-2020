#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <cinttypes>

#if __has_include(<format>)
# include <format>
#else
# include <fmt/core.h>
namespace std {
	template <typename... Ts>
	decltype(auto) format (Ts&& ... args) {
		return (fmt::format(std::forward<Ts>(args)...));
	}
}
#endif

using u32 = uint32_t;
using i32 = int32_t;

using u64 = uint64_t;
using i64 = int64_t;

#define SU32 "%" SCNu32
#define SU64 "%" SCNu64
