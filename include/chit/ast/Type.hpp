#pragma once

#include <chit/ast/Node.hpp>
#include <chit/util/Json.hpp>

#include <string_view>

namespace chit {
	class IdentifierTypeNode final : public TypeNode {
	public:
		std::u8string_view Name;

	public:
		explicit IdentifierTypeNode(std::u8string_view name) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;
	};
}