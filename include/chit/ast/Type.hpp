#pragma once

#include <chit/ast/Node.hpp>
#include <chit/util/Json.hpp>

#include <string_view>
#include <vector>

namespace chit {
	class IdentifierTypeNode final : public TypeNode {
	public:
		std::vector<std::u8string_view> Names;

	public:
		explicit IdentifierTypeNode(std::u8string_view name);
		explicit IdentifierTypeNode(
			std::vector<std::u8string_view> names) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void Analyze(ParserContext& context) const override;

	private:
		void DetermineBuiltinType() const;
	};
}