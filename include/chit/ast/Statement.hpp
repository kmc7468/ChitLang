#pragma once

#include <chit/ast/Node.hpp>
#include <chit/util/Json.hpp>

#include <memory>

namespace chit {
	class ReturnNode final : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> Expression;

	public:
		explicit ReturnNode(std::unique_ptr<ExpressionNode> expression) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void Generate(GeneratorContext& context) const override;
	};
}