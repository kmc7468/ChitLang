#pragma once

#include <chit/Assembly.hpp>

#include <memory>
#include <vector>

namespace chit {
	class Context;

	class Node {
	public:
		Node() noexcept = default;
		Node(const Node&) = delete;
		virtual ~Node() = default;

	public:
		Node& operator=(const Node&) = delete;

	public:
		virtual void Generate(Context& context, BodyStream* stream) const = 0;
	};

	class ExpressionNode : public Node {};
	class StatementNode : public Node {};

	class TypeNode : public Node {
	public:
		virtual bool IsVoid() const noexcept;
	};
}

namespace chit {
	class RootNode final : public StatementNode {
	public:
		std::vector<std::unique_ptr<StatementNode>> Statements;

	public:
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};
}

namespace chit {
	class EmptyStatementNode final : public StatementNode {
	public:
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};;

	class ExpressionStatementNode final : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> Expression;

	public:
		explicit ExpressionStatementNode(std::unique_ptr<ExpressionNode> expression) noexcept;

	public:
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};

	class BlockNode : public StatementNode {
	public:
		std::vector<std::unique_ptr<StatementNode>> Statements;

	public:
		explicit BlockNode(std::vector<std::unique_ptr<StatementNode>> statements) noexcept;

	public:
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};
}