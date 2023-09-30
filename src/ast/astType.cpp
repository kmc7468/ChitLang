#include <chit/ast/Type.hpp>

#include <chit/Type.hpp>

#include <cassert>

namespace chit {
	IdentifierTypeNode::IdentifierTypeNode(std::u8string_view name) noexcept
		: Name(name) {
		
		assert(!name.empty());
	}

	void IdentifierTypeNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"IdentifierTypeNode\",\"name\":\"" << Name << u8"\",";

		TypeNode::DumpJson(stream);

		stream << u8"}";
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