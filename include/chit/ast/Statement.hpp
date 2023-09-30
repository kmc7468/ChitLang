#pragma once

#include <chit/ast/Node.hpp>

#include <memory>

namespace chit {
	class ReturnNode final : public StatementNode {
	public:
		std::unique_ptr<ExpressionNode> Expression;

	public:
		explicit ReturnNode(std::unique_ptr<ExpressionNode> expression) noexcept;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};
}