#pragma once

#include <chit/ast/Node.hpp>

namespace chit {
	class BuiltinTypeNode : public TypeNode {
	public:
		virtual void Generate(Context& context, BodyStream* stream) override;
	};
}