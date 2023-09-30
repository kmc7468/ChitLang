#include <chit/ast/Type.hpp>

#include <chit/Type.hpp>

#include <cassert>

namespace chit {
	IdentifierTypeNode::IdentifierTypeNode(std::u8string_view name) noexcept
		: Name(name) {
		
		assert(!name.empty());
	}

	JsonValue IdentifierTypeNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"IdentifierTypeNode").
			SetField(u8"name", std::u8string(Name)).
			Merge(TypeNode::DumpJson()).
			Build();
	}
	void IdentifierTypeNode::Analyze(ParserContext&) const {
		if (Name == u8"void") {
			Type = BuiltinType::Void;
		} else if (Name == u8"int") {
			Type = BuiltinType::Int;
		} else {
			// TODO
		}
	}
}