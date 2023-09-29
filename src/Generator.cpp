#include <chit/Generator.hpp>

namespace chit {
	const Node* Context::FindSymbol(std::u8string_view name) const noexcept {
		if (const auto iterator = Symbols.find(name); iterator != Symbols.end()) {
			return iterator->second;
		} else if (Parent) {
			return Parent->FindSymbol(name);
		} else {
			return nullptr;
		}
	}
}