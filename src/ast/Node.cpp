#include <chit/ast/Node.hpp>

#include <chit/Generator.hpp>
#include <chit/ast/Type.hpp>

#include <cassert>
#include <utility>

namespace chit {
	namespace {
		static const IdentifierTypeNode s_VoidType(u8"void");
		static const IdentifierTypeNode s_IntType(u8"int");
	}

	const TypeNode* const VoidType = &s_VoidType;
	const TypeNode* const IntType = &s_IntType;
}

namespace chit {
	void ExpressionNode::GenerateAssignment(Context&, BodyStream*, const ExpressionNode*) const {
		assert(false);
	}
	void ExpressionNode::GenerateFunctionCall(Context&, BodyStream*) const {
		assert(false);
	}
}

namespace chit {
	void RootNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"RootNode\",\"statements\":[";

		bool isFirst = true;

		for (const auto& statement : Statements) {
			if (!isFirst) {
				stream << u8',';
			} else {
				isFirst = false;
			}

			statement->DumpJson(stream);
		}

		stream << u8"]}";
	}
	void RootNode::Generate(Context& context, BodyStream* stream) const {
		for (auto& statement : Statements) {
			statement->Generate(context, stream);
		}
	}
}

namespace chit {
	void EmptyStatementNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"EmptyStatementNode\"}";
	}
	void EmptyStatementNode::Generate(Context&, BodyStream*) const {}
}

namespace chit {
	ExpressionStatementNode::ExpressionStatementNode(
		std::unique_ptr<ExpressionNode> expression) noexcept

		: Expression(std::move(expression)) {}

	void ExpressionStatementNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"ExpressionStatementNode\",\"expression\":";
		Expression->DumpJson(stream);
		stream << u8'}';
	}
	void ExpressionStatementNode::Generate(Context& context, BodyStream* stream) const {
		Expression->Generate(context, stream);
		*stream << u8"pop\n";
	}
}

namespace chit {
	BlockNode::BlockNode(std::vector<std::unique_ptr<StatementNode>> statements) noexcept
		: Statements(std::move(statements)) {}

	void BlockNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"BlockNode\",\"statements\":[";

		bool isFirst = true;

		for (const auto& statement : Statements) {
			if (!isFirst) {
				stream << u8',';
			} else {
				isFirst = false;
			}

			statement->DumpJson(stream);
		}

		stream << u8"]}";
	}
	void BlockNode::Generate(Context& context, BodyStream* stream) const {
		Context blockContext{
			.Assembly = context.Assembly,
			.Messages = context.Messages,
			.Parent = &context,
		};

		for (auto& statement : Statements) {
			statement->Generate(blockContext, stream);
		}
	}
}