#include <chit/ast/Statement.hpp>

#include <cassert>
#include <utility>

namespace chit {
	JsonValue EmptyStatementNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"EmptyStatementNode").
			Build();
	}
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
}

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
}