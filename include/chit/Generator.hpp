#pragma once

#include <chit/Assembly.hpp>

#include <string_view>
#include <unordered_map>

namespace chit {
	class Node;

	class Context final {
	public:
		chit::Assembly& Assembly;
		Context* Parent = nullptr;

		std::unordered_map<std::u8string_view, const Node*> Symbols;

	public:
		const Node* FindSymbol(std::u8string_view name) const noexcept;
	};
}