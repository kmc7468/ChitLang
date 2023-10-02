#include <chit/ast/Node.hpp>

#include <chit/Parser.hpp>

#include <cassert>
#include <utility>

namespace chit {
	JsonValue TypeNode::DumpJson() const {
		return JsonObject().
			SetField(u8"type", Type ? Type->DumpJson() : JsonNull()).
			Build();
	}
}

namespace chit {
	JsonValue ExpressionNode::DumpJson() const {
		return JsonObject().
			SetField(u8"type", Type ? Type->DumpJson() : JsonNull()).
			SetField(u8"isLValue", IsLValue).
			Build();
	}
}

namespace chit {
	JsonValue RootNode::DumpJson() const {
		JsonArray statements;

		for (const auto& statement : Statements) {
			statements.AddElement(statement->DumpJson());
		}

		return JsonObject().
			SetField(u8"class", u8"RootNode").
			SetField(u8"statements", statements.Build()).
			Build();
	}
}

namespace chit {
	BlockNode::BlockNode(std::vector<std::unique_ptr<StatementNode>> statements) noexcept
		: Statements(std::move(statements)) {}

	JsonValue BlockNode::DumpJson() const {
		JsonArray statements;

		for (const auto& statement : Statements) {
			statements.AddElement(statement->DumpJson());
		}

		return JsonObject().
			SetField(u8"class", u8"BlockNode").
			SetField(u8"statements", statements.Build()).
			Build();
	}
}