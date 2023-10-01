#include <chit/ast/Statement.hpp>

#include <chit/Generator.hpp>
#include <chit/Parser.hpp>

#include <cassert>
#include <utility>

namespace chit {
	ReturnNode::ReturnNode(std::unique_ptr<ExpressionNode> expression) noexcept
		: Expression(std::move(expression)) {

		assert(Expression);
	}

	JsonValue ReturnNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"ReturnNode").
			SetField(u8"expression", Expression->DumpJson()).
			Build();
	}
	void ReturnNode::Analyze(ParserContext& context) const {
		Expression->Analyze(context);

		// TODO: Type checking

		FunctionReturnType = context.FunctionReturnType;
	}
	void ReturnNode::Generate(GeneratorContext& context) const {
		assert(context.Stream);

		Expression->GenerateValue(context);

		if (Expression->IsLValue) {
			*context.Stream << u8"tload\n";
		}
		if (!FunctionReturnType->IsEqual(Expression->Type)) {
			FunctionReturnType->GenerateConvert(context);
		}

		*context.Stream << u8"ret\n";
	}
}

namespace chit {
	IfNode::IfNode(
		std::unique_ptr<ExpressionNode> condition,
		std::unique_ptr<StatementNode> body,
		std::unique_ptr<StatementNode> elseBody) noexcept

		: Condition(std::move(condition)), Body(std::move(body)),
		ElseBody(std::move(elseBody)) {

		assert(Condition);
		assert(Body);
	}

	JsonValue IfNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"IfNode").
			SetField(u8"condition", Condition->DumpJson()).
			SetField(u8"body", Body->DumpJson()).
			SetField(u8"elseBody", ElseBody ? ElseBody->DumpJson() : JsonNull()).
			Build();
	}
	void IfNode::Analyze(ParserContext& context) const {
		Condition->Analyze(context);

		// TODO: Type checking

		Body->Analyze(context);

		if (ElseBody) {
			ElseBody->Analyze(context);
		}
	}
	void IfNode::Generate(GeneratorContext& context) const {
		assert(context.Stream);

		Condition->GenerateValue(context);

		if (Condition->IsLValue) {
			*context.Stream << u8"tload\n";
		}

		const auto jumpLabelName = context.CreateTempIdentifier();
		const auto doneLabelName = context.CreateTempIdentifier();

		*context.Stream << u8"jne " << jumpLabelName << u8'\n';

		if (ElseBody) {
			ElseBody->Generate(context);
		}

		*context.Stream <<
			u8"jmp " << doneLabelName << u8'\n' <<
			jumpLabelName << u8":\n";

		Body->Generate(context);

		*context.Stream << doneLabelName << u8":\n";
	}
}