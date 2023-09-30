#include <chit/ast/Statement.hpp>

#include <chit/Generator.hpp>

#include <cassert>
#include <utility>

namespace chit {
	ReturnNode::ReturnNode(std::unique_ptr<ExpressionNode> expression) noexcept
		: Expression(std::move(expression)) {

		assert(Expression);
	}

	JsonValue ReturnNode::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"ReturnNode").
			SetField(u8"expression", Expression->DumpJson()).
			Build();
	}
	void ReturnNode::Analyze(ParserContext& context) const {
		Expression->Analyze(context);

		// TODO: Type checking
	}
	void ReturnNode::Generate(GeneratorContext& context) const {
		assert(context.Stream);

		Expression->GenerateValue(context);

		if (Expression->IsLValue) {
			*context.Stream << u8"tload\n";
		}

		*context.Stream << u8"ret\n";
	}
}