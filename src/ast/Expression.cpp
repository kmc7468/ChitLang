#include <chit/ast/Expression.hpp>

#include <chit/Generator.hpp>
#include <chit/ast/Declaration.hpp>
#include <chit/util/String.hpp>

namespace chit {
	IdentifierNode::IdentifierNode(std::u8string_view name) noexcept
		: Name(name) {}

	void IdentifierNode::Generate(Context& context, BodyStream* stream) const {
		if (const auto defNode = context.FindSymbol(Name); defNode) {
			if (!dynamic_cast<const VariableDeclarationNode*>(defNode)) {
				// TODO
			}

			*stream << u8"load " << Name << u8'\n';
		} else {
			// TODO
		}
	}
}

namespace chit {
	IntConstantNode::IntConstantNode(std::int32_t value) noexcept
		: Value(value) {}

	void IntConstantNode::Generate(Context&, BodyStream* stream) const {
		*stream << u8"push " << ToUtf8String(Value) << u8'\n';
	}
}