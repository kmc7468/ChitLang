#include <chit/ast/Node.hpp>

#include <chit/Generator.hpp>

#include <utility>

namespace chit {
	void RootNode::Generate(Context& context, BodyStream* stream) const {
		for (auto& statement : Statements) {
			statement->Generate(context, stream);
		}
	}
}

namespace chit {
	bool TypeNode::IsVoid() const noexcept {
		return false;
	}
}

namespace chit {
	void EmptyStatementNode::Generate(Context&, BodyStream*) const {}
}

namespace chit {
	ExpressionStatementNode::ExpressionStatementNode(
		std::unique_ptr<ExpressionNode> expression) noexcept

		: Expression(std::move(expression)) {}

	void ExpressionStatementNode::Generate(Context& context, BodyStream* stream) const {
		Expression->Generate(context, stream);
		*stream << u8"pop\n";
	}
}

namespace chit {
	BlockNode::BlockNode(std::vector<std::unique_ptr<StatementNode>> statements) noexcept
		: Statements(std::move(statements)) {}

	void BlockNode::Generate(Context& context, BodyStream* stream) const {
		Context blockContext{
			.Assembly = context.Assembly,
			.Parent = &context,
		};

		for (auto& statement : Statements) {
			statement->Generate(blockContext, stream);
		}
	}
}