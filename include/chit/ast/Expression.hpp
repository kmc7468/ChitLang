#pragma once

#include <chit/Token.hpp>
#include <chit/ast/Node.hpp>

#include <cstdint>
#include <string_view>

namespace chit {
	class IdentifierNode final : public ExpressionNode {
	public:
		std::u8string_view Name;

	public:
		explicit IdentifierNode(std::u8string_view name) noexcept;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Generate(Context& context, BodyStream* stream) const override;
		virtual void GenerateAssignment(
			Context& context,
			BodyStream* stream,
			const ExpressionNode* right) const override;

		virtual bool IsLValue() const noexcept override;
	};
}

namespace chit {
	class IntConstantNode final : public ExpressionNode {
	public:
		std::int32_t Value;

	public:
		explicit IntConstantNode(std::int32_t value) noexcept;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Generate(Context& context, BodyStream* stream) const override;

		virtual bool IsLValue() const noexcept override;
	};
}

namespace chit {
	class BinaryOperatorNode final : public ExpressionNode {
	public:
		TokenType Operator;
		std::unique_ptr<ExpressionNode> Left;
		std::unique_ptr<ExpressionNode> Right;

	public:
		explicit BinaryOperatorNode(
			TokenType operator_,
			std::unique_ptr<ExpressionNode> left,
			std::unique_ptr<ExpressionNode> right) noexcept;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Generate(Context& context, BodyStream* stream) const override;

		virtual bool IsLValue() const noexcept override;
	};
}