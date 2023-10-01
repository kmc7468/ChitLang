#include <chit/ast/Declaration.hpp>

#include <chit/Assembly.hpp>
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

	JsonValue FunctionDeclarationNode::DumpJson() const {
		JsonArray parameters;

		for (const auto& [name, value] : Parameters) {
			parameters.AddElement(JsonObject().
				SetField(u8"name", std::u8string(name)).
				SetField(u8"type", value->DumpJson()).
				Build());
		}

		return JsonObject().
			SetField(u8"class", u8"FunctionDeclarationNode").
			SetField(u8"returnType", ReturnType->DumpJson()).
			SetField(u8"name", std::u8string(Name)).
			SetField(u8"parameters", parameters.Build()).
			Build();
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

	JsonValue FunctionDefinitionNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"FunctionDefinitionNode").
			SetField(u8"prototype", Prototype->DumpJson()).
			SetField(u8"body", Body->DumpJson()).
			Build();
	}
	void FunctionDefinitionNode::Analyze(ParserContext& context) const {
		if (!context.SymbolTable.IsGlobal()) {
			// TODO: Error
		}

		Prototype->Analyze(context);

		context.FunctionReturnType = Prototype->ReturnType->Type;

		Body->Analyze(context);

		context.FunctionReturnType = nullptr;
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
			bodyStream << u8"push 0i\n";
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

	JsonValue VariableDeclarationNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"VariableDeclarationNode").
			SetField(u8"type", Type->DumpJson()).
			SetField(u8"name", std::u8string(Name)).
			SetField(u8"initializer", Initializer ? Initializer->DumpJson() : JsonNull()).
			Build();
	}
	void VariableDeclarationNode::Analyze(ParserContext& context) const {
		Type->Analyze(context);

		if (Initializer) {
			Initializer->Analyze(context);

			// TODO: Type checking
		}

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

			if (!Type->Type->IsEqual(Initializer->Type)) {
				Type->Type->GenerateConvert(context);
			}

			*context.Stream << u8"store " << Name << u8'\n';
		}
	}
}