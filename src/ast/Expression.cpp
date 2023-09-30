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

	void IdentifierNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"IdentifierNode\",\"name\":\"" << Name << u8"\",";

		ExpressionNode::DumpJson(stream);

		stream << u8'}';
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
							   u8"lea " << Name << u8'\n';
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

	void IntConstantNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"IntConstantNode\",\"value\":" << ToUtf8String(Value) << u8',';

		ExpressionNode::DumpJson(stream);

		stream << u8'}';
	}
	void IntConstantNode::Analyze(ParserContext&) const {
		Type = BuiltinType::Int;
		IsLValue = false;
	}
	void IntConstantNode::GenerateValue(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		*context.Stream << u8"push " << ToUtf8String(Value) << u8'\n';
	}
}

namespace chit {
	BinaryOperatorNode::BinaryOperatorNode(
		TokenType operator_,
		std::unique_ptr<ExpressionNode> left,
		std::unique_ptr<ExpressionNode> right) noexcept

		: Operator(operator_), Left(std::move(left)), Right(std::move(right)) {

		assert(Operator == TokenType::Assignment);
		assert(Left);
		assert(Right);
	}

	void BinaryOperatorNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"BinaryOperatorNode\",\"operator\":\"" << char8_t(Operator) << u8"\",\"left\":";

		Left->DumpJson(stream);

		stream << u8",\"right\":";

		Right->DumpJson(stream);

		stream << u8',';

		ExpressionNode::DumpJson(stream);

		stream << u8'}';
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
			IsLValue = true;

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
		}
	}
	void BinaryOperatorNode::GenerateAssignment(GeneratorContext& context) const {
		assert(Type);
		assert(context.Stream);

		if (Operator != TokenType::Assignment) {
			// TODO: Error
		}

		const auto rhsTempName = context.CreateTempIdentifier();
		const auto lhsTempName = context.CreateTempIdentifier();

		*context.Stream << u8"store " << rhsTempName << u8'\n';

		GenerateValue(context);

		*context.Stream << u8"store " << lhsTempName << u8'\n' <<
						   u8"load " << rhsTempName << u8'\n' <<
						   u8"load " << lhsTempName << u8'\n' <<
						   u8"tstore\n" <<
						   u8"load " << lhsTempName << u8'\n';
	}
}

namespace chit {
	FunctionCallNode::FunctionCallNode(
		std::unique_ptr<ExpressionNode> function,
		std::vector<std::unique_ptr<ExpressionNode>> arguments) noexcept

		: Function(std::move(function)), Arguments(std::move(arguments)) {

		assert(Function);
	}

	void FunctionCallNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"FunctionCallNode\",\"function\":";

		Function->DumpJson(stream);

		stream << u8",\"arguments\":[";

		bool isFirst = true;

		for (const auto& argument : Arguments) {
			if (!isFirst) {
				stream << u8',';
			} else {
				isFirst = false;
			}

			argument->DumpJson(stream);
		}

		stream << u8"],";

		ExpressionNode::DumpJson(stream);

		stream << u8'}';
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