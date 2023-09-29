#include <chit/ast/Function.hpp>

#include <chit/Generator.hpp>

#include <algorithm>
#include <iterator>

namespace chit {
	void FunctionDeclarationNode::Generate(Context& context, BodyStream*) const {
		auto& declaration = context.Symbols[Name];

		if (!declaration) {
			// TODO
		}

		declaration = this;
	}
}

namespace chit {
	void FunctionDefinitionNode::Generate(Context& context, BodyStream*) const {
		Prototype->Generate(context, nullptr);

		std::vector<std::u8string_view> parameterNames;
		std::transform(
			Prototype->Parameters.begin(),
			Prototype->Parameters.end(),
			std::back_inserter(parameterNames),
			[](const auto& parameter) {
				return parameter.first;
			});

		auto& bodyStream = context.Assembly.AddFunction(
			Prototype->Name,
			!Prototype->ReturnType->IsVoid(),
			std::move(parameterNames));

		Body->Generate(context, &bodyStream);
	}
}