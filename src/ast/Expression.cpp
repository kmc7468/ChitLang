#include <chit/ast/Expression.hpp>

#include <chit/Generator.hpp>
#include <chit/Parser.hpp>
#include <chit/Type.hpp>
#include <chit/util/String.hpp>

#include <cassert>
#include <ranges>
#include <variant>

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
	void IdentifierNode::Analyze(ParserContext& context) const {
		if (const auto symbol = context.SymbolTable.FindSymbol(Name);
			symbol) {

			if (const auto varSymbol = IsVariableSymbol(symbol->first);
				varSymbol) {

				Type = varSymbol->Type;
				IsLValue = true;

				Symbol = symbol->first;
			} else if (const auto funcSymbol = IsFunctionSymbol(symbol->first);
					   funcSymbol) {

				Type = funcSymbol->Type;
				IsLValue = true; // Function is considered as lvalue in ChitLang

				Symbol = symbol->first;
			}
		} else {
			// TODO: Error
		}
	}
	void IdentifierNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		if (IsVariableSymbol(Symbol)) {
			*context.Stream << u8"lea " << Name << u8'\n';
		} else {
			// TODO: Error
		}
	}
	void IdentifierNode::GenerateAssignment(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		if (IsVariableSymbol(Symbol)) {
			*context.Stream << u8"store " << Name << u8'\n' <<
							   u8"load " << Name << u8'\n';
		} else {
			// TODO: Error
		}
	}
	void IdentifierNode::GenerateFunctionCall(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		if (IsFunctionSymbol(Symbol)) {
			*context.Stream << u8"call " << Name << u8'\n';

			if (std::static_pointer_cast<FunctionType>(Type)->ReturnType ==
				BuiltinType::Void) {

				*context.Stream << u8"push 0\n"; // Temp value
			}
		} else {
			// TODO: Error
		}
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
	void IntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::Int;
		IsLValue = false;
	}
	void IntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"i\n";
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
	void UnsignedIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::UnsignedInt;
		IsLValue = false;
	}
	void UnsignedIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"i\n";
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
	void LongIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::LongInt;
		IsLValue = false;
	}
	void LongIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"i\n";
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
	void UnsignedLongIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::UnsignedLongInt;
		IsLValue = false;
	}
	void UnsignedLongIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"i\n";
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
	void LongLongIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::LongLongInt;
		IsLValue = false;
	}
	void LongLongIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"l\n";
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
	void UnsignedLongLongIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::UnsignedLongLongInt;
		IsLValue = false;
	}
	void UnsignedLongLongIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"l\n";
	}
}

namespace chit {
	BinaryOperatorNode::BinaryOperatorNode(
		TokenType operator_,
		std::unique_ptr<ExpressionNode> left,
		std::unique_ptr<ExpressionNode> right) noexcept

		: Operator(operator_), Left(std::move(left)), Right(std::move(right)) {

		assert(
			Operator == TokenType::Assignment		||
			Operator == TokenType::Addition			||
			Operator == TokenType::Subtraction		||
			Operator == TokenType::Multiplication	||
			Operator == TokenType::Division			||
			Operator == TokenType::Modulo);
		assert(Left);
		assert(Right);
	}

	JsonValue BinaryOperatorNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"BinaryOperatorNode").
			SetField(u8"operator", std::u8string(1, char8_t(Operator))).
			SetField(u8"left", Left->DumpJson()).
			SetField(u8"right", Right->DumpJson()).
			Merge(ExpressionNode::DumpJson()).
			Build();
	}
	void BinaryOperatorNode::Analyze(ParserContext& context) const {
		Left->Analyze(context);
		Right->Analyze(context);

		// TODO: Type checking

		switch (Operator) {
		case TokenType::Assignment:
			if (!Left->IsLValue) {
				// TODO: Error
			}

			Type = Left->Type;
			IsLValue = false;

		case TokenType::Addition:
		case TokenType::Subtraction:
		case TokenType::Multiplication:
		case TokenType::Division:
		case TokenType::Modulo:
			Type = Left->Type; // TODO
			IsLValue = false;

			break;
		}
	}
	void BinaryOperatorNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		switch (Operator) {
		case TokenType::Assignment:
			Right->GenerateValue(context);

			if (Right->IsLValue) {
				*context.Stream << u8"tload\n";
			}

			Left->GenerateAssignment(context);

			break;

		case TokenType::Addition:
		case TokenType::Subtraction:
		case TokenType::Multiplication:
		case TokenType::Division:
		case TokenType::Modulo:
			Left->GenerateValue(context);
			Right->GenerateValue(context);

			switch (Operator) {
			case TokenType::Addition:
				*context.Stream << u8"add\n";

				break;

			case TokenType::Subtraction:
				*context.Stream << u8"sub\n";

				break;

			case TokenType::Multiplication:
				*context.Stream << u8"imul\n"; // TODO

				break;

			case TokenType::Division:
				*context.Stream << u8"idiv\n"; // TODO

				break;

			case TokenType::Modulo:
				*context.Stream << u8"imod\n"; // TODO

				break;
			}

			break;
		}
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
	void FunctionCallNode::Analyze(ParserContext& context) const {
		Function->Analyze(context);

		for (const auto& argument : Arguments) {
			argument->Analyze(context);
		}

		Type = std::dynamic_pointer_cast<FunctionType>(Function->Type)->ReturnType;
		IsLValue = false;
	}
	void FunctionCallNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		for (const auto& argument : Arguments | std::views::reverse) {
			argument->GenerateValue(context);

			if (argument->IsLValue) {
				*context.Stream << u8"tload\n";
			}
		}

		Function->GenerateFunctionCall(context);
	}
}