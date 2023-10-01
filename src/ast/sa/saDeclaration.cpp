#include <chit/ast/Declaration.hpp>

#include <chit/Parser.hpp>

namespace chit {
	void FunctionDeclarationNode::Analyze(ParserContext& context) const {
		ReturnType->Analyze(context);

		std::vector<TypePtr> parameterTypes;

		for (const auto& parameter : Parameters) {
			parameter.second->Analyze(context);

			parameterTypes.push_back(parameter.second->Type);
		}

		Symbol = IsFunctionSymbol(context.SymbolTable.CreateFunctionSymbol(
			Name,
			ReturnType->Type,
			std::move(parameterTypes)));
		if (!Symbol) {
			// TODO: Error or ignore
		}
	}
}

namespace chit {
	void FunctionDefinitionNode::Analyze(chit::ParserContext& context) const {
		if (!context.SymbolTable.IsGlobal()) {
			// TODO: Error
		}

		Prototype->Analyze(context);

		ParserContext = std::unique_ptr<chit::ParserContext>(new chit::ParserContext{
			.Messages = context.Messages,
			.SymbolTable = SymbolTable(context.SymbolTable),
			.FunctionReturnType = Prototype->ReturnType->Type,
		});

		for (const auto& parameter : Prototype->Parameters) {
			ParserContext->SymbolTable.CreateVariableSymbol(
				parameter.first,
				parameter.second->Type,
				VariableState::Initalized
			);
		}

		Body->Analyze(*ParserContext);
	}
}

namespace chit {
	void VariableDeclarationNode::Analyze(ParserContext& context) const {
		Type->Analyze(context);

		if (Initializer) {
			Initializer->Analyze(context);

			// TODO: Type checking
		}

		Symbol = IsVariableSymbol(context.SymbolTable.CreateVariableSymbol(
			Name,
			Type->Type,
			Initializer ? VariableState::Initalized : VariableState::Uninitialized)
		);

		if (!Symbol) {
			// TODO: Error or ignore
		}
	}
}