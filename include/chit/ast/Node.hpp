#pragma once

#include <chit/Assembly.hpp>
#include <chit/Type.hpp>

#include <memory>
#include <vector>

namespace chit {
	struct ParserContext;

	class Node {
	public:
		Node() noexcept = default;
		Node(const Node&) = delete;
		virtual ~Node() = default;

	public:
		Node& operator=(const Node&) = delete;

	public:
		virtual void DumpJson(BodyStream& stream) const = 0;
		virtual void Analyze(ParserContext& context) const = 0;
	};
}

namespace chit {
	class TypeNode : public Node {
	public:
		mutable TypePtr Type;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
	};
}

namespace chit {
	class GeneratorContext;

	class ExpressionNode : public Node {
	public:
		mutable TypePtr Type;
		mutable bool IsLValue = false;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void GenerateValue(GeneratorContext& context) const = 0;
		virtual void GenerateAssignment(GeneratorContext& context) const;
		virtual void GenerateFunctionCall(GeneratorContext& context) const;
	};
}

namespace chit {
	class GeneratorContext;

	class StatementNode : public Node {
	public:
		virtual void Generate(GeneratorContext& context) const = 0;
	};

	class RootNode final : public StatementNode {
	public:
		std::vector<std::unique_ptr<StatementNode>> Statements;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};

	class EmptyStatementNode final : public StatementNode {
	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};

	class ExpressionStatementNode final : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> Expression;

	public:
		explicit ExpressionStatementNode(std::unique_ptr<ExpressionNode> expression) noexcept;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};

	class BlockNode final : public StatementNode {
	public:
		std::vector<std::unique_ptr<StatementNode>> Statements;

		mutable std::unique_ptr<chit::ParserContext> ParserContext;

	public:
		explicit BlockNode(std::vector<std::unique_ptr<StatementNode>> statements) noexcept;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Analyze(chit::ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};
}