#include <chit/ast/Expression.hpp>

#include <chit/Generator.hpp>
#include <chit/ast/Declaration.hpp>
#include <chit/util/String.hpp>

#include <cassert>

namespace chit {
	IdentifierNode::IdentifierNode(std::u8string_view name) noexcept
		: Name(name) {

		assert(!Name.empty());
	}

	void IdentifierNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"IdentifierNode\",\"name\":\"" << Name << u8"\"}";
	}
	void IdentifierNode::Generate(Context& context, BodyStream* stream) const {
		assert(stream);

		if (const auto defNode = context.FindSymbol(Name); defNode) {
			if (!dynamic_cast<const VariableDeclarationNode*>(defNode)) {
				// TODO
			}

			*stream << u8"lea " << Name << u8'\n';
		} else {
			// TODO
		}
	}
	void IdentifierNode::GenerateAssignment(
		Context& context,
		BodyStream* stream,
		const ExpressionNode* right) const {

		assert(stream);

		if (const auto defNode = context.FindSymbol(Name); defNode) {
			if (!dynamic_cast<const VariableDeclarationNode*>(defNode)) {
				// TODO
			}

			right->Generate(context, stream);

			if (right->IsLValue()) {
				*stream << u8"tload\n";
			}

			*stream << u8"store " << Name << u8'\n'
					<< u8"lea " << Name << u8'\n';
		} else {
			// TODO
		}
	}

	bool IdentifierNode::IsLValue() const noexcept {
		return true;
	}
}

namespace chit {
	IntConstantNode::IntConstantNode(std::int32_t value) noexcept
		: Value(value) {}

	void IntConstantNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"IntConstantNode\",\"value\":" << ToUtf8String(Value) << u8'}';
	}
	void IntConstantNode::Generate(Context&, BodyStream* stream) const {
		assert(stream);

		*stream << u8"push " << ToUtf8String(Value) << u8'\n';
	}

	bool IntConstantNode::IsLValue() const noexcept {
		return false;
	}
}

namespace chit {
	BinaryOperatorNode::BinaryOperatorNode(
		TokenType operator_,
		std::unique_ptr<ExpressionNode> left,
		std::unique_ptr<ExpressionNode> right) noexcept

		: Operator(operator_), Left(std::move(left)), Right(std::move(right)) {

		assert(Operator == TokenType::Assignment);
		assert(Left);
		assert(Right);
	}

	void BinaryOperatorNode::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"BinaryOperatorNode\",\"operator\":\"" << char8_t(Operator) << u8"\",\"left\":";

		Left->DumpJson(stream);

		stream << u8",\"right\":";

		Right->DumpJson(stream);

		stream << u8'}';
	}
	void BinaryOperatorNode::Generate(Context& context, BodyStream* stream) const {
		assert(stream);

		switch (Operator) {
		case TokenType::Assignment:
			if (Left->IsRValue()) {
				// TODO
			}

			Left->GenerateAssignment(context, stream, Right.get());

			break;
		}
	}

	bool BinaryOperatorNode::IsLValue() const noexcept {
		switch (Operator) {
		case TokenType::Assignment:
			return true;

		default:
			return false;
		}
	}
}