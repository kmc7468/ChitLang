#include <chit/ast/Expression.hpp>

#include <chit/Generator.hpp>
#include <chit/Parser.hpp>
#include <chit/Type.hpp>
#include <chit/util/String.hpp>

#include <cassert>
#include <string>

namespace chit {
	IdentifierNode::IdentifierNode(std::u8string_view name) noexcept
		: Name(name) {

		assert(!Name.empty());
	}

	JsonValue IdentifierNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"IdentifierNode").
			SetField(u8"name", std::u8string(Name)).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}

namespace chit {
	IntConstantNode::IntConstantNode(std::int32_t value) noexcept
		: Value(value) {}

	JsonValue IntConstantNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"IntConstantNode").
			SetField(u8"value", static_cast<std::int64_t>(Value)).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}

namespace chit {
	UnsignedIntConstantNode::UnsignedIntConstantNode(std::uint32_t value) noexcept
		: Value(value) {}

	JsonValue UnsignedIntConstantNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"UnsignedIntConstantNode").
			SetField(u8"value", static_cast<std::uint64_t>(Value)).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}

namespace chit {
	LongIntConstantNode::LongIntConstantNode(std::int32_t value) noexcept
		: Value(value) {}

	JsonValue LongIntConstantNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"LongIntConstantNode").
			SetField(u8"value", static_cast<std::int64_t>(Value)).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}

namespace chit {
	UnsignedLongIntConstantNode::UnsignedLongIntConstantNode(std::uint32_t value) noexcept
		: Value(value) {}

	JsonValue UnsignedLongIntConstantNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"UnsignedLongIntConstantNode").
			SetField(u8"value", static_cast<std::uint64_t>(Value)).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}

namespace chit {
	LongLongIntConstantNode::LongLongIntConstantNode(std::int64_t value) noexcept
		: Value(value) {}

	JsonValue LongLongIntConstantNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"LongLongIntConstantNode").
			SetField(u8"value", Value).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}

namespace chit {
	UnsignedLongLongIntConstantNode::UnsignedLongLongIntConstantNode(std::uint64_t value) noexcept
		: Value(value) {}

	JsonValue UnsignedLongLongIntConstantNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"UnsignedLongLongIntConstantNode").
			SetField(u8"value", Value).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}

namespace chit {
	BinaryOperatorNode::BinaryOperatorNode(
		TokenType operator_,
		std::unique_ptr<ExpressionNode> left,
		std::unique_ptr<ExpressionNode> right) noexcept

		: Operator(operator_), Left(std::move(left)), Right(std::move(right)) {

		assert(
			Operator == TokenType::Assignment			||

			Operator == TokenType::Addition				||
			Operator == TokenType::Subtraction			||
			Operator == TokenType::Multiplication		||
			Operator == TokenType::Division				||
			Operator == TokenType::Modulo				||
		
			Operator == TokenType::Equivalence			||
			Operator == TokenType::GreaterThan			||
			Operator == TokenType::LessThan				||
			Operator == TokenType::GreaterThanOrEqual	||
			Operator == TokenType::LessThanOrEqual);
		assert(Left);
		assert(Right);
	}

	JsonValue BinaryOperatorNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"BinaryOperatorNode").
			SetField(u8"operator", std::u8string(TokenSymbols.at(Operator))).
			SetField(u8"left", Left->DumpJson()).
			SetField(u8"right", Right->DumpJson()).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}

namespace chit {
	FunctionCallNode::FunctionCallNode(
		std::unique_ptr<ExpressionNode> function,
		std::vector<std::unique_ptr<ExpressionNode>> arguments) noexcept

		: Function(std::move(function)), Arguments(std::move(arguments)) {

		assert(Function);
	}

	JsonValue FunctionCallNode::DumpJson() const {
		JsonArray arguments;

		for (const auto& argument : Arguments) {
			arguments.AddElement(argument->DumpJson());
		}

		return JsonObject().
			SetField(u8"class", u8"FunctionCallNode").
			SetField(u8"function", Function->DumpJson()).
			SetField(u8"arguments", arguments.Build()).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
}