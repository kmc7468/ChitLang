#include <chit/ast/Node.hpp>

#include <chit/Symbol.hpp>
#include <chit/Parser.hpp>

namespace chit {
	void RootNode::Analyze(ParserContext& context) const {
		for (auto& statement : Statements) {
			statement->Analyze(context);
		}
	}
}

namespace chit {
	void EmptyStatementNode::Analyze(ParserContext&) const {}
}

namespace chit {
	void ExpressionStatementNode::Analyze(ParserContext& context) const {
		Expression->Analyze(context);
	}
}

namespace chit {
	void BlockNode::Analyze(chit::ParserContext& context) const {
		ParserContext = std::unique_ptr<chit::ParserContext>(new chit::ParserContext{
			.Messages = context.Messages,
			.SymbolTable = SymbolTable(context.SymbolTable),
			.FunctionReturnType = context.FunctionReturnType,
		});

		for (auto& statement : Statements) {
			statement->Analyze(*ParserContext);
		}
	}
}