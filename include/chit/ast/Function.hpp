#pragma once

#include <chit/Assembly.hpp>
#include <chit/ast/Node.hpp>

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

	public:
		FunctionDeclarationNode(
			std::unique_ptr<TypeNode> returnType,
			std::u8string_view name,
			std::vector<std::pair<
				std::u8string_view,
				std::unique_ptr<TypeNode>>> parameters) noexcept;

	public:
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};
}

namespace chit {
	class FunctionDefinitionNode final : public StatementNode {
	public:
		std::unique_ptr<FunctionDeclarationNode> Prototype;
		std::unique_ptr<BlockNode> Body;

	public:
		FunctionDefinitionNode(
			std::unique_ptr<FunctionDeclarationNode> prototype,
			std::unique_ptr<BlockNode> body) noexcept;

	public:
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};
}