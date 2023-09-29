#include <chit/ast/Type.hpp>

namespace chit {
	IdentifierTypeNode::IdentifierTypeNode(std::u8string_view name) noexcept
		: Name(name) {}

	void IdentifierTypeNode::Generate(Context&, BodyStream*) const {}

	bool IdentifierTypeNode::IsVoid() const noexcept {
		using namespace std::string_view_literals;

		return Name == u8"void"sv;
	}
}