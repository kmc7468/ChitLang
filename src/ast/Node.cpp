#include <chit/ast/Node.hpp>

#include <chit/Generator.hpp>

namespace chit {
	void RootNode::Generate(Context& context, BodyStream* stream) const {
		for (auto& statement : Statements) {
			statement->Generate(context, stream);
		}
	}

	bool TypeNode::IsVoid() const noexcept {
		using namespace std::string_view_literals;

		return Name == u8"void"sv;
	}
}

namespace chit {
	void BlockNode::Generate(Context& context, BodyStream* stream) const {
		Context blockContext{
			.Assembly = context.Assembly,
			.Parent = &context,
		};

		for (auto& statement : Statements) {
			statement->Generate(blockContext, stream);
		}
	}
}