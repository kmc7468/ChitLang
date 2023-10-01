#include <chit/ast/Type.hpp>

#include <chit/Type.hpp>

namespace chit {
	void IdentifierTypeNode::Analyze(ParserContext&) const {
		if (Names[0] == u8"void") {
			Type = BuiltinType::Void;
		} else {
			DetermineBuiltinType();
		}
	}

	void IdentifierTypeNode::DetermineBuiltinType() const {
		int intCount = 0, longCount = 0;
		bool isSigned = false, isUnsigned = false;

		for (const auto& name : Names) {
			if (name == u8"int") {
				++intCount;

				if (intCount >= 2) {
					// TODO: Error
					return;
				}
			} else if (name == u8"long") {
				++longCount;

				if (longCount >= 3) {
					// TODO: Error
					return;
				}
			} else if (name == u8"signed") {
				isSigned = true;

				if (isUnsigned) {
					// TODO: Error
					return;
				}
			} else if (name == u8"unsigned") {
				isUnsigned = true;

				if (isSigned) {
					// TODO: Error
					return;
				}
			}
		}

		if (intCount == 0 && longCount == 0) {
			intCount = 1;
		} else if (!isSigned && !isUnsigned) {
			isSigned = true;
		}

		if (longCount == 2) {
			Type = isSigned ? BuiltinType::LongLongInt : BuiltinType::UnsignedLongLongInt;
		} else if (longCount == 1) {
			Type = isSigned ? BuiltinType::LongInt : BuiltinType::UnsignedLongInt;
		} else {
			Type = isSigned ? BuiltinType::Int : BuiltinType::UnsignedInt;
		}
	}
}