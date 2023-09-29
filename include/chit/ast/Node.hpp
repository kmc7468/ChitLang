#pragma once

#include <memory>
#include <string_view>
#include <vector>

namespace chit {
	struct Context;

	class Node {
	public:
		Node() noexcept = default;
		Node(const Node&) = delete;
		virtual ~Node() = default;

	public:
		Node& operator=(const Node&) = delete;

	public:
		virtual void Generate(Context& context, BodyStream* stream) = 0;
	};

	class ExpressionNode : public Node {};
	class StatementNode : public Node {};
	class TypeNode : public Node {
	public:
		std::u8string_view Name;

	public:
		bool IsVoid() const noexcept;
	};
}

namespace chit {
	class RootNode final : public StatementNode {
	public:
		std::vector<std::unique_ptr<StatementNode>> Statements;

	public:
		virtual void Generate(Context& context, BodyStream* stream) override;
	};
}

namespace chit {
	class BlockNode : public StatementNode {
	public:
		std::vector<std::unique_ptr<StatementNode>> Statements;

	public:
		virtual void Generate(Context& context, BodyStream* stream) override;
	};
}