#include <chit/ast/Expression.hpp>

#include <chit/Generator.hpp>
#include <chit/util/String.hpp>

#include <cassert>
#include <memory>
#include <ranges>
#include <unordered_map>

namespace chit {
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
			*context.Stream <<
				u8"store " << Name << u8'\n' <<
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
		} else {
			// TODO: Error
		}
	}
}

namespace chit {
	void IntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"i\n";
	}
}

namespace chit {
	void UnsignedIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"i\n";
	}
}

namespace chit {
	void LongIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"i\n";
	}
}

namespace chit {
	void UnsignedLongIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"i\n";
	}
}

namespace chit {
	void LongLongIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"l\n";
	}
}

namespace chit {
	void UnsignedLongLongIntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8"l\n";
	}
}

namespace chit {
	void BinaryOperatorNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		switch (Operator) {
		case TokenType::Assignment:
			Right->GenerateValue(context);

			if (Right->IsLValue) {
				*context.Stream << u8"tload\n";
			}
			if (NewRightType) {
				NewRightType->GenerateConvert(context);
			}

			Left->GenerateAssignment(context);

			break;

		case TokenType::Addition:
		case TokenType::Subtraction:
		case TokenType::Multiplication:
		case TokenType::Division:
		case TokenType::Modulo:
		case TokenType::Equivalence:
		case TokenType::GreaterThan:
		case TokenType::LessThan:
		case TokenType::GreaterThanOrEqual:
		case TokenType::LessThanOrEqual: {

			Left->GenerateValue(context);

			if (Left->IsLValue) {
				*context.Stream << u8"tload\n";
			}
			if (NewLeftType) {
				NewLeftType->GenerateConvert(context);
			}

			Right->GenerateValue(context);

			if (Right->IsLValue) {
				*context.Stream << u8"tload\n";
			}
			if (NewRightType) {
				NewRightType->GenerateConvert(context);
			}

			const auto isUnsigned = IsBuiltinType(OperandType)->IsUnsigned();

			switch (Operator) {
			case TokenType::Addition:
				*context.Stream << u8"add\n"; break;
			case TokenType::Subtraction:
				*context.Stream << u8"sub\n"; break;
			case TokenType::Multiplication:
				*context.Stream << (isUnsigned ? u8"mul\n" : u8"imul\n"); break;
			case TokenType::Division:
				*context.Stream << (isUnsigned ? u8"div\n" : u8"idiv\n"); break;
			case TokenType::Modulo:
				*context.Stream << (isUnsigned ? u8"mod\n" : u8"imod\n"); break;

			case TokenType::Equivalence:
			case TokenType::GreaterThan:
			case TokenType::LessThan:
			case TokenType::GreaterThanOrEqual:
			case TokenType::LessThanOrEqual: {

				const auto jumpLabelName = context.CreateTempIdentifier();
				const auto doneLabelName = context.CreateTempIdentifier();

				static const std::unordered_map<
					TokenType,
					std::u8string_view> jumpInstructions{

					{ TokenType::Equivalence, u8"je" },
					{ TokenType::GreaterThan, u8"ja" },
					{ TokenType::LessThan, u8"jb" },
					{ TokenType::GreaterThanOrEqual, u8"jae" },
					{ TokenType::LessThanOrEqual, u8"jbe" },
				};

				*context.Stream <<
					(isUnsigned ? u8"cmp\n" : u8"icmp\n") <<
					jumpInstructions.at(Operator) << u8' ' << jumpLabelName << u8'\n' <<
					u8"pop\n" <<
					u8"push 0i\n" <<
					u8"jmp " << doneLabelName << u8'\n' <<
					jumpLabelName << u8":\n" <<
					u8"push 1i\n" <<
					doneLabelName << u8":\n";

				break;
			}
			}
		}
		}
	}
}

namespace chit {
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