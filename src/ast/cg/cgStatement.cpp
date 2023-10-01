#include <chit/ast/Statement.hpp>

#include <chit/Generator.hpp>

#include <cassert>

namespace chit {
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
	void IfNode::Generate(GeneratorContext& context) const {
		assert(context.Stream);

		Condition->GenerateValue(context);

		if (Condition->IsLValue) {
			*context.Stream << u8"tload\n";
		}

		const auto jumpLabelName = context.CreateTempIdentifier();
		const auto doneLabelName = context.CreateTempIdentifier();

		*context.Stream <<
			u8"jne " << jumpLabelName << u8'\n' <<
			u8"pop\n";

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