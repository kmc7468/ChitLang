#include <chit/ast/Declaration.hpp>

#include <chit/Generator.hpp>

#include <algorithm>
#include <iterator>

namespace chit {
	FunctionDeclarationNode::FunctionDeclarationNode(
		std::unique_ptr<TypeNode> returnType,
		std::u8string_view name,
		std::vector<std::pair<
			std::u8string_view,
			std::unique_ptr<TypeNode>>> parameters) noexcept

		: ReturnType(std::move(returnType)), Name(name),
		Parameters(std::move(parameters)) {}

	void FunctionDeclarationNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"FunctionDeclarationNode\",\"returnType\":";

		ReturnType->DumpJson(stream);

		stream << u8",\"name\":\"" << Name << u8"\",\"parameters\":[";

		bool isFirst = true;

		for (const auto& [name, type] : Parameters) {
			if (!isFirst) {
				stream << u8',';
			} else {
				isFirst = false;
			}

			stream << u8"{\"name\":\"" << name << u8"\",\"type\":";

			type->DumpJson(stream);

			stream << u8'}';
		}

		stream << u8"]}";
	}
	void FunctionDeclarationNode::Generate(Context& context, BodyStream*) const {
		auto& declaration = context.Symbols[Name];

		if (declaration) {
			// TODO
		}

		declaration = this;
	}
}

namespace chit {
	FunctionDefinitionNode::FunctionDefinitionNode(
		std::unique_ptr<FunctionDeclarationNode> prototype,
		std::unique_ptr<BlockNode> body) noexcept

		: Prototype(std::move(prototype)), Body(std::move(body)) {}

	void FunctionDefinitionNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"FunctionDefinitionNode\",\"prototype\":";

		Prototype->DumpJson(stream);

		stream << u8",\"body\":";

		Body->DumpJson(stream);

		stream << u8'}';
	}
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

		bodyStream << u8"ret\n";
	}
}

namespace chit {
	VariableDeclarationNode::VariableDeclarationNode(
		std::unique_ptr<TypeNode> type,
		std::u8string_view name,
		std::unique_ptr<ExpressionNode> initializer) noexcept

		: Type(std::move(type)), Name(std::move(name)),
		Initializer(std::move(initializer)) {}

	void VariableDeclarationNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"VariableDeclarationNode\",\"type\":";
		Type->DumpJson(stream);

		stream << u8",\"name\":\"" << Name << u8"\",\"initializer\":";

		if (Initializer) {
			Initializer->DumpJson(stream);
		} else {
			stream << u8"null";
		}

		stream << u8'}';
	}
	void VariableDeclarationNode::Generate(Context& context, BodyStream* stream) const {
		auto& declaration = context.Symbols[Name];

		if (declaration) {
			// TODO
		}

		declaration = this;

		if (!Initializer)
			return;

		Initializer->Generate(context, stream);
		*stream << u8"store " << Name << u8'\n';
	}
}