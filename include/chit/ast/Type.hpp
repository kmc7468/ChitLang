#pragma once

#include <chit/Assembly.hpp>
#include <chit/ast/Node.hpp>

namespace chit {
	class BuiltinTypeNode final : public TypeNode {
	public:
		virtual void Generate(Context& context, BodyStream* stream) const override;
	};
}