#pragma once

#include <chit/Symbol.hpp>
#include <chit/Token.hpp>
#include <chit/ast/Node.hpp>
#include <chit/util/Json.hpp>

#include <cstdint>
#include <string_view>

namespace chit {
	class IdentifierNode final : public ExpressionNode {
	public:
		std::u8string_view Name;

		mutable chit::Symbol* Symbol = nullptr;

	public:
		explicit IdentifierNode(std::u8string_view name) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
		virtual void GenerateAssignment(GeneratorContext& context) const override;
		virtual void GenerateFunctionCall(GeneratorContext& context) const override;
	};
}

namespace chit {
	class IntConstantNode final : public ExpressionNode {
	public:
		std::int32_t Value;

	public:
		explicit IntConstantNode(std::int32_t value) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
	};

	class UnsignedIntConstantNode final : public ExpressionNode {
	public:
		std::uint32_t Value;

	public:
		explicit UnsignedIntConstantNode(std::uint32_t value) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
	};

	class LongIntConstantNode final : public ExpressionNode {
	public:
		std::int32_t Value;

	public:
		explicit LongIntConstantNode(std::int32_t value) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
	};

	class UnsignedLongIntConstantNode final : public ExpressionNode {
	public:
		std::uint32_t Value;

	public:
		explicit UnsignedLongIntConstantNode(std::uint32_t value) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
	};

	class LongLongIntConstantNode final : public ExpressionNode {
	public:
		std::int64_t Value;

	public:
		explicit LongLongIntConstantNode(std::int64_t value) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
	};

	class UnsignedLongLongIntConstantNode final : public ExpressionNode {
	public:
		std::uint64_t Value;

	public:
		explicit UnsignedLongLongIntConstantNode(std::uint64_t value) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
	};
}

namespace chit {
	class BinaryOperatorNode final : public ExpressionNode {
	public:
		TokenType Operator;
		std::unique_ptr<ExpressionNode> Left;
		std::unique_ptr<ExpressionNode> Right;

		mutable TypePtr OperandType;
		mutable TypePtr NewLeftType;
		mutable TypePtr NewRightType;

	public:
		explicit BinaryOperatorNode(
			TokenType operator_,
			std::unique_ptr<ExpressionNode> left,
			std::unique_ptr<ExpressionNode> right) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
	};
}

namespace chit {
	class FunctionCallNode final : public ExpressionNode {
	public:
		std::unique_ptr<ExpressionNode> Function;
		std::vector<std::unique_ptr<ExpressionNode>> Arguments;

	public:
		explicit FunctionCallNode(
			std::unique_ptr<ExpressionNode> function,
			std::vector<std::unique_ptr<ExpressionNode>> arguments) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
		virtual void GenerateValue(GeneratorContext& context) const override;
	};
}