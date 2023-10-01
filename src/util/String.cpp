#include <chit/util/String.hpp>

namespace chit {
	std::u8string ToUtf8String(std::int32_t integer) {
		return ToUtf8String(static_cast<std::int64_t>(integer));
	}
	std::u8string ToUtf8String(std::uint32_t integer) {
		return ToUtf8String(static_cast<std::uint64_t>(integer));
	}
	std::u8string ToUtf8String(std::int64_t integer) {
		const auto temp = std::to_string(integer);

		return { temp.begin(), temp.end() };
	}
	std::u8string ToUtf8String(std::uint64_t integer) {
		const auto temp = std::to_string(integer);

		return { temp.begin(), temp.end() };
	}
}