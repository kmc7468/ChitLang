#pragma once

#include <cstdint>
#include <string>

namespace chit {
	std::u8string ToUtf8String(std::int32_t integer);
	std::u8string ToUtf8String(std::uint32_t integer);
	std::u8string ToUtf8String(std::int64_t integer);
	std::u8string ToUtf8String(std::uint64_t integer);
}