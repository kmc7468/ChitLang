#include <chit/ast/Node.hpp>

#include <chit/Generator.hpp>
#include <chit/Parser.hpp>

#include <cassert>
#include <utility>

namespace chit {
	JsonValue TypeNode::DumpJson() const {
		return JsonObject().
			SetField(u8"type", Type ? Type->DumpJson() : JsonNull()).
			Build();
	}
}

namespace chit {
	JsonValue ExpressionNode::DumpJson() const {
		return JsonObject().
			SetField(u8"type", Type ? Type->DumpJson() : JsonNull()).
			SetField(u8"isLValue", IsLValue).
			Build();
	}
	void ExpressionNode::GenerateAssignment(GeneratorContext&) const {
		assert(false);
	}
	void ExpressionNode::GenerateFunctionCall(GeneratorContext&) const {
		assert(false);
	}
}

namespace chit {
	JsonValue RootNode::DumpJson() const {
		JsonArray statements;

		for (const auto& statement : Statements) {
			statements.AddElement(statement->DumpJson());
		}

		return JsonObject().
			SetField(u8"class", u8"RootNode").
			SetField(u8"statements", statements.Build()).
			Build();
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
	JsonValue EmptyStatementNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"EmptyStatementNode").
			Build();
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

	JsonValue ExpressionStatementNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"ExpressionStatementNode").
			SetField(u8"expression", Expression->DumpJson()).
			Build();
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

	JsonValue BlockNode::DumpJson() const {
		JsonArray statements;

		for (const auto& statement : Statements) {
			statements.AddElement(statement->DumpJson());
		}

		return JsonObject().
			SetField(u8"class", u8"BlockNode").
			SetField(u8"statements", statements.Build()).
			Build();
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