#include <chit/ast/Expression.hpp>

#include <chit/Parser.hpp>
#include <chit/Type.hpp>

namespace chit {
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
}

namespace chit {
	void IntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::Int;
		IsLValue = false;
	}
}

namespace chit {
	void UnsignedIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::UnsignedInt;
		IsLValue = false;
	}
}

namespace chit {
	void LongIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::LongInt;
		IsLValue = false;
	}
}

namespace chit {
	void UnsignedLongIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::UnsignedLongInt;
		IsLValue = false;
	}
}

namespace chit {
	void LongLongIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::LongLongInt;
		IsLValue = false;
	}
}

namespace chit {
	void UnsignedLongLongIntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::UnsignedLongLongInt;
		IsLValue = false;
	}
}

namespace chit {
	void BinaryOperatorNode::Analyze(ParserContext& context) const {
		Left->Analyze(context);
		Right->Analyze(context);

		// TODO: Type checking

		switch (Operator) {
		case TokenType::Assignment:
			if (!Left->IsLValue) {
				// TODO: Error
			}

			if (!Left->Type->IsEqual(Right->Type)) {
				// TODO: Type checking

				NewRightType = Left->Type;
			}

			Type = Left->Type;
			IsLValue = false;

		case TokenType::Addition:
		case TokenType::Subtraction:
		case TokenType::Multiplication:
		case TokenType::Division:
		case TokenType::Modulo:
		case TokenType::Equivalence:
		case TokenType::GreaterThan:
		case TokenType::LessThan:
		case TokenType::GreaterThanOrEqual:
		case TokenType::LessThanOrEqual:
			if (Left->Type->IsVoid() || !IsBuiltinType(Left->Type) ||
				Right->Type->IsVoid() || !IsBuiltinType(Right->Type)) {

				// TODO: Error
			}

			BuiltinType::RunIntegerPromotion(NewLeftType, Left->Type);
			BuiltinType::RunIntegerPromotion(NewRightType, Right->Type);

			OperandType = BuiltinType::RunUsualArithmeticConversion(
				NewLeftType, NewLeftType ? NewLeftType : Left->Type,
				NewRightType, NewRightType ? NewRightType : Right->Type);

			if (Operator == TokenType::Equivalence			||
				Operator == TokenType::GreaterThan			||
				Operator == TokenType::LessThan				||
				Operator == TokenType::GreaterThanOrEqual	||
				Operator == TokenType::LessThanOrEqual) {

				Type = BuiltinType::Int;
				IsLValue = false;
			} else {
				Type = OperandType;
				IsLValue = false;
			}

			break;
		}
	}
}

namespace chit {
	void FunctionCallNode::Analyze(ParserContext& context) const {
		Function->Analyze(context);

		for (const auto& argument : Arguments) {
			argument->Analyze(context);
		}

		Type = IsFunctionType(Function->Type)->ReturnType;
		IsLValue = false;
	}
}