#include <chit/Type.hpp>

#include <algorithm>
#include <cassert>
#include <utility>

namespace chit {
	bool Type::IsEqual(const TypePtr& other) const noexcept {
		return this == other.get();
	}
	bool Type::IsVoid() const noexcept {
		return false;
	}
}

namespace chit {
	const std::shared_ptr<BuiltinType> BuiltinType::Void =
		std::make_shared<BuiltinType>(u8"void");
	const std::shared_ptr<BuiltinType> BuiltinType::Int =
		std::make_shared<BuiltinType>(u8"int");

	BuiltinType::BuiltinType(std::u8string_view name) noexcept
		: Name(name) {

		assert(!Name.empty());
	}

	void BuiltinType::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"BuiltinType\",\"name\":\"" << Name << u8"\"}";
	}

	bool BuiltinType::IsVoid() const noexcept {
		return Name == u8"void";
	}
}

namespace chit {
	FunctionType::FunctionType(
		TypePtr returnType,
		std::vector<TypePtr> parameterTypes) noexcept

		: ReturnType(std::move(returnType)),
		ParameterTypes(std::move(parameterTypes)) {

		assert(ReturnType);
	}

	void FunctionType::DumpJson(BodyStream& stream) const {
		stream << u8"{\"class\":\"FunctionType\",\"returnType\":";

		ReturnType->DumpJson(stream);

		stream << u8",\"parameterTypes\":[";

		bool isFirst = true;

		for (const auto& paramType : ParameterTypes) {
			if (!isFirst) {
				stream << u8',';
			} else {
				isFirst = false;
			}

			paramType->DumpJson(stream);
		}

		stream << u8"]}";
	}

	bool FunctionType::IsEqual(const TypePtr& other) const noexcept {
		if (const auto otherFunction = std::dynamic_pointer_cast<FunctionType>(other);
			otherFunction) {

			return
				ReturnType->IsEqual(otherFunction->ReturnType) &&
				ParameterTypes.size() == otherFunction->ParameterTypes.size() &&
				std::equal(
					ParameterTypes.begin(), ParameterTypes.end(),
					otherFunction->ParameterTypes.begin(),
					[](const auto& a, const auto& b) {
						return a->IsEqual(b);
					});
		} else {
			return false;
		}
	}
}