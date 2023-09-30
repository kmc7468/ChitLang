#include <chit/ast/Node.hpp>

#include <chit/Generator.hpp>
#include <chit/Parser.hpp>

#include <cassert>
#include <utility>

namespace chit {
	void TypeNode::DumpJson(BodyStream& stream) const {
		stream << u8"\"type\":";

		if (Type) {
			Type->DumpJson(stream);
		} else {
			stream << u8"null";
		}
	}
}

namespace chit {
	void ExpressionNode::DumpJson(BodyStream& stream) const {
		stream << u8"\"type\":";

		if (Type) {
			Type->DumpJson(stream);
		} else {
			stream << u8"null";
		}

		stream << u8",\"isLValue\":" << (IsLValue ? u8"true" : u8"false");
	}
	void ExpressionNode::GenerateAssignment(GeneratorContext&) const {
		assert(false);
	}
	void ExpressionNode::GenerateFunctionCall(GeneratorContext&) const {
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
	void RootNode::Analyze(ParserContext& context) const {
		for (auto& statement : Statements) {
			statement->Analyze(context);
		}
	}
	void RootNode::Generate(GeneratorContext& context) const {
		for (auto& statement : Statements) {
			statement->Generate(context);
		}
	}
}

namespace chit {
	void EmptyStatementNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"EmptyStatementNode\"}";
	}
	void EmptyStatementNode::Analyze(ParserContext&) const {}
	void EmptyStatementNode::Generate(GeneratorContext&) const {}
}

namespace chit {
	ExpressionStatementNode::ExpressionStatementNode(
		std::unique_ptr<ExpressionNode> expression) noexcept

		: Expression(std::move(expression)) {

		assert(Expression);
	}

	void ExpressionStatementNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"ExpressionStatementNode\",\"expression\":";

		Expression->DumpJson(stream);

		stream << u8'}';
	}
	void ExpressionStatementNode::Analyze(ParserContext& context) const {
		Expression->Analyze(context);
	}
	void ExpressionStatementNode::Generate(GeneratorContext& context) const {
		assert(context.Stream);

		Expression->GenerateValue(context);

		*context.Stream << u8"pop\n";
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
	void BlockNode::Analyze(chit::ParserContext& context) const {
		ParserContext = std::unique_ptr<chit::ParserContext>(new chit::ParserContext{
			.Messages = context.Messages,
			.SymbolTable = SymbolTable(context.SymbolTable),
		});

		for (auto& statement : Statements) {
			statement->Analyze(*ParserContext);
		}
	}
	void BlockNode::Generate(GeneratorContext& context) const {
		assert(ParserContext);

		GeneratorContext blockContext{
			.Parent = &context,
			.Assembly = context.Assembly,
			.Stream = context.Stream,
			.Messages = context.Messages,
		};

		for (auto& statement : Statements) {
			statement->Generate(blockContext);
		}
	}
}