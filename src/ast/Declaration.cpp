#include <chit/ast/Declaration.hpp>

#include <chit/Generator.hpp>
#include <chit/Parser.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>

namespace chit {
	FunctionDeclarationNode::FunctionDeclarationNode(
		std::unique_ptr<TypeNode> returnType,
		std::u8string_view name,
		std::vector<std::pair<
			std::u8string_view,
			std::unique_ptr<TypeNode>>> parameters) noexcept

		: ReturnType(std::move(returnType)), Name(name),
		Parameters(std::move(parameters)) {

		assert(ReturnType);
		assert(!Name.empty());
	}

	void FunctionDeclarationNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"FunctionDeclarationNode\",\"returnType\":";

		ReturnType->DumpJson(stream);

		stream << u8",\"name\":\"" << Name << u8"\",\"parameters\":[";

		bool isFirst = true;

		for (const auto& [name, type] : Parameters) {
			if (!isFirst) {
				stream << u8',';
			} else {
				isFirst = false;
			}

			stream << u8"{\"name\":\"" << name << u8"\",\"type\":";

			type->DumpJson(stream);

			stream << u8'}';
		}

		stream << u8"]}";
	}
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
	void FunctionDeclarationNode::Generate(GeneratorContext&) const {
		assert(Symbol);
	}
}

namespace chit {
	FunctionDefinitionNode::FunctionDefinitionNode(
		std::unique_ptr<FunctionDeclarationNode> prototype,
		std::unique_ptr<BlockNode> body) noexcept

		: Prototype(std::move(prototype)), Body(std::move(body)) {

		assert(Prototype);
		assert(Body);
	}

	void FunctionDefinitionNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"FunctionDefinitionNode\",\"prototype\":";

		Prototype->DumpJson(stream);

		stream << u8",\"body\":";

		Body->DumpJson(stream);

		stream << u8'}';
	}
	void FunctionDefinitionNode::Analyze(ParserContext& context) const {
		Prototype->Analyze(context);
		Body->Analyze(context);

		if (!context.SymbolTable.IsGlobal()) {
			// TODO: Error
		}
	}
	void FunctionDefinitionNode::Generate(chit::GeneratorContext& context) const {
		Prototype->Generate(context);

		std::vector<std::u8string_view> parameterNames;
		std::transform(
			Prototype->Parameters.begin(),
			Prototype->Parameters.end(),
			std::back_inserter(parameterNames),
			[](const auto& parameter) {
				return parameter.first;
			});

		auto& bodyStream = context.Assembly.AddFunction(
			Prototype->Name,
			!Prototype->ReturnType->Type->IsVoid(),
			std::move(parameterNames));

		GeneratorContext defContext{
			.Parent = &context,
			.Assembly = context.Assembly,
			.Stream = &bodyStream,
			.Messages = context.Messages,
		};

		Body->Generate(defContext);

		if (Prototype->Name == u8"main") {
			bodyStream << u8"push 0\n";
		}

		bodyStream << u8"ret\n";
	}
}

namespace chit {
	VariableDeclarationNode::VariableDeclarationNode(
		std::unique_ptr<TypeNode> type,
		std::u8string_view name,
		std::unique_ptr<ExpressionNode> initializer) noexcept

		: Type(std::move(type)), Name(std::move(name)),
		Initializer(std::move(initializer)) {

		assert(Type);
		assert(!Name.empty());
	}

	void VariableDeclarationNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"VariableDeclarationNode\",\"type\":";
		Type->DumpJson(stream);

		stream << u8",\"name\":\"" << Name << u8"\",\"initializer\":";

		if (Initializer) {
			Initializer->DumpJson(stream);
		} else {
			stream << u8"null";
		}

		stream << u8'}';
	}
	void VariableDeclarationNode::Analyze(ParserContext& context) const {
		Type->Analyze(context);
		Initializer->Analyze(context);

		Symbol = IsVariableSymbol(context.SymbolTable.CreateVariableSymbol(
			Name,
			Type->Type,
			Initializer ? VariableState::Initalized : VariableState::Uninitialized));
		if (!Symbol) {
			// TODO: Error or ignore
		}
	}
	void VariableDeclarationNode::Generate(GeneratorContext& context) const {
		assert(Symbol);
		assert(context.Stream);

		if (Initializer) {
			Initializer->GenerateValue(context);

			*context.Stream << u8"store " << Name << u8'\n';
		}
	}
}