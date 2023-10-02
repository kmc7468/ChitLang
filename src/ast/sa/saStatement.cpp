#include <chit/ast/Statement.hpp>

#include <chit/Parser.hpp>

namespace chit {
	void EmptyStatementNode::Analyze(ParserContext&) const {}
}

namespace chit {
	void ExpressionStatementNode::Analyze(ParserContext& context) const {
		Expression->Analyze(context);
	}
}

namespace chit {
	void ReturnNode::Analyze(ParserContext& context) const {
		Expression->Analyze(context);

		// TODO: Type checking

		FunctionReturnType = context.FunctionReturnType;
	}
}

namespace chit {
	void IfNode::Analyze(ParserContext& context) const {
		Condition->Analyze(context);

		// TODO: Type checking

		Body->Analyze(context);

		if (ElseBody) {
			ElseBody->Analyze(context);
		}
	}
}