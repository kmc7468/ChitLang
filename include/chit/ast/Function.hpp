#pragma once

#include <chit/ast/Node.hpp>

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace chit {
	class FunctionDeclarationNode final : public StatementNode {
	public:
		TypeNode* ReturnType;
		std::u8string_view Name;
		std::vector<std::pair<std::u8string_view, TypeNode*>> Parameters;

	public:
		virtual void Generate(Context& context, BodyStream* stream) override;
	};
}

namespace chit {
	class FunctionDefinitionNode final : public StatementNode {
	public:
		std::unique_ptr<FunctionDeclarationNode> Prototype;
		std::unique_ptr<BlockNode> Body;

	public:
		virtual void Generate(Context& context, BodyStream* stream) override;
	};
}