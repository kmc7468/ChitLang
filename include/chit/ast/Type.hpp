#pragma once

#include <chit/Assembly.hpp>
#include <chit/ast/Node.hpp>

#include <string_view>

namespace chit {
	class IdentifierTypeNode final : public TypeNode {
	public:
		std::u8string_view Name;

	public:
		explicit IdentifierTypeNode(std::u8string_view name) noexcept;

	public:
		virtual void DumpJson(BodyStream& stream) const override;
		virtual void Generate(Context& context, BodyStream* stream) const override;

		virtual bool IsVoid() const noexcept override;
	};
}