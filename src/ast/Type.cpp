#include <chit/ast/Type.hpp>

namespace chit {
	IdentifierTypeNode::IdentifierTypeNode(std::u8string_view name) noexcept
		: Name(name) {}

	void IdentifierTypeNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"IdentifierTypeNode\",\"name\":\"" << Name << u8"\"}";
	}
	void IdentifierTypeNode::Generate(Context&, BodyStream*) const {}

	bool IdentifierTypeNode::IsVoid() const noexcept {
		return Name == u8"void";
	}
}