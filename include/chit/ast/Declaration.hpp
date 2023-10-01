#pragma once

#include <chit/Symbol.hpp>
#include <chit/ast/Node.hpp>
#include <chit/util/Json.hpp>

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace chit {
	class FunctionDeclarationNode final : public StatementNode {
	public:
		std::unique_ptr<TypeNode> ReturnType;
		std::u8string_view Name;
		std::vector<std::pair<
			std::u8string_view,
			std::unique_ptr<TypeNode>>> Parameters;

		mutable FunctionSymbol* Symbol = nullptr;

	public:
		FunctionDeclarationNode(
			std::unique_ptr<TypeNode> returnType,
			std::u8string_view name,
			std::vector<std::pair<
				std::u8string_view,
				std::unique_ptr<TypeNode>>> parameters) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};

	class FunctionDefinitionNode final : public StatementNode {
	public:
		std::unique_ptr<FunctionDeclarationNode> Prototype;
		std::unique_ptr<BlockNode> Body;

		mutable std::unique_ptr<chit::ParserContext> ParserContext;

	public:
		FunctionDefinitionNode(
			std::unique_ptr<FunctionDeclarationNode> prototype,
			std::unique_ptr<BlockNode> body) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(chit::ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};
}

namespace chit {
	class VariableDeclarationNode final : public StatementNode {
	public:
		std::unique_ptr<TypeNode> Type;
		std::u8string_view Name;
		std::unique_ptr<ExpressionNode> Initializer;

		mutable VariableSymbol* Symbol = nullptr;

	public:
		VariableDeclarationNode(
			std::unique_ptr<TypeNode> type,
			std::u8string_view name,
			std::unique_ptr<ExpressionNode> initializer = nullptr) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};
}