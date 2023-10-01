#pragma once

#include <chit/Type.hpp>
#include <chit/ast/Node.hpp>
#include <chit/util/Json.hpp>

#include <memory>

namespace chit {
	class ReturnNode final : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> Expression;

		mutable TypePtr FunctionReturnType;

	public:
		explicit ReturnNode(std::unique_ptr<ExpressionNode> expression) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};
}

namespace chit {
	class IfNode final : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> Condition;
		std::unique_ptr<StatementNode> Body;
		std::unique_ptr<StatementNode> ElseBody;

	public:
		IfNode(
			std::unique_ptr<ExpressionNode> condition,
			std::unique_ptr<StatementNode> body,
			std::unique_ptr<StatementNode> elseBody) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};
}