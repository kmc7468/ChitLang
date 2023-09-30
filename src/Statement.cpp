#include <chit/ast/Statement.hpp>

#include <utility>

namespace chit {
	ReturnNode::ReturnNode(std::unique_ptr<ExpressionNode> expression) noexcept
		: Expression(std::move(expression)) {}

	void ReturnNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"ReturnNode\",\"expression\":";

		Expression->DumpJson(stream);

		stream << u8'}';
	}
	void ReturnNode::Generate(Context& context, BodyStream* stream) const {
		Expression->Generate(context, stream);

		if (Expression->IsLValue()) {
			*stream << u8"tload\n";
		}

		*stream << u8"ret\n";
	}
}