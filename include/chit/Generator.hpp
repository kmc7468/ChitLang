#pragma once

#include <chit/Assembly.hpp>

#include <string_view>
#include <unordered_map>

namespace chit {
	class Node;

	struct Context final {
		chit::Assembly& Assembly;
		Context* Parent = nullptr;

		std::unordered_map<std::u8string_view, const Node*> Symbols;
	};
}