#include <chit/util/String.hpp>

namespace chit {
	std::u8string ToUtf8String(std::int32_t integer) {
		const auto temp = std::to_string(integer);

		return { temp.begin(), temp.end() };
	}
}