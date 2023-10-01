#include <chit/ast/Type.hpp>

#include <chit/Type.hpp>

#include <cassert>
#include <string>
#include <utility>

namespace chit {
	IdentifierTypeNode::IdentifierTypeNode(std::u8string_view name)
		: Names{ name } {

		assert(!Names[0].empty());
	}
	IdentifierTypeNode::IdentifierTypeNode(
		std::vector<std::u8string_view> names) noexcept

		: Names(std::move(names)) {

		assert(!Names.empty());
		assert(!Names[0].empty());
	}

	JsonValue IdentifierTypeNode::DumpJson() const {
		JsonArray names;

		for (const auto& name : Names) {
			names.AddElement(std::u8string(name));
		}

		return JsonObject().
			SetField(u8"class", u8"IdentifierTypeNode").
			SetField(u8"names", names.Build()).
			Merge(TypeNode::DumpJson()).
			Build();
	}
}