#include <chit/ast/Declaration.hpp>

#include <chit/Parser.hpp>

#include <cassert>
#include <string>

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
}