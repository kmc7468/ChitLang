#include <chit/ast/Node.hpp>

#include <chit/Generator.hpp>

#include <cassert>

namespace chit {
	void ExpressionNode::GenerateAssignment(GeneratorContext&) const {
		assert(false);
	}
	void ExpressionNode::GenerateFunctionCall(GeneratorContext&) const {
		assert(false);
	}
}

namespace chit {
	void RootNode::Generate(GeneratorContext& context) const {
		for (auto& statement : Statements) {
			statement->Generate(context);
		}
	}
}

namespace chit {
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