#pragma once

#include <cstddef>
#include <optional>
#include <string>

namespace chit {
	enum class MessageType {
		Error,
	};

	struct Message final {
		MessageType Type;
		std::u8string Data;
		std::size_t Line, Column;
	};
}