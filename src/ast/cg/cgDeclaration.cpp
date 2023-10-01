#include <chit/ast/Declaration.hpp>

#include <chit/Generator.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>

namespace chit {
	void FunctionDeclarationNode::Generate(GeneratorContext&) const {
		assert(Symbol);
	}
}

namespace chit {
	void FunctionDefinitionNode::Generate(chit::GeneratorContext& context) const {
		Prototype->Generate(context);

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
			!Prototype->ReturnType->Type->IsVoid(),
			std::move(parameterNames));

		GeneratorContext defContext{
			.Parent = &context,
			.Assembly = context.Assembly,
			.Stream = &bodyStream,
			.Messages = context.Messages,
		};

		Body->Generate(defContext);

		if (Prototype->Name == u8"main") {
			bodyStream << u8"push 0i\n";
		}

		bodyStream << u8"ret\n";
	}
}

namespace chit {
	void VariableDeclarationNode::Generate(GeneratorContext& context) const {
		assert(Symbol);
		assert(context.Stream);

		if (Initializer) {
			Initializer->GenerateValue(context);

			if (!Type->Type->IsEqual(Initializer->Type)) {
				Type->Type->GenerateConvert(context);
			}

			*context.Stream << u8"store " << Name << u8'\n';
		}
	}
}