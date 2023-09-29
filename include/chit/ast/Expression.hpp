#pragma once

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
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};
}

namespace chit {
	class IntConstantNode final : public ExpressionNode {
	public:
		std::int32_t Value;

	public:
		explicit IntConstantNode(std::int32_t value) noexcept;

	public:
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};
}