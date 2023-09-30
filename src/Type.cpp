#include <chit/Type.hpp>

#include <algorithm>
#include <cassert>
#include <string>
#include <utility>

namespace chit {
	bool Type::IsEqual(const std::shared_ptr<Type>& other) const noexcept {
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
	const std::shared_ptr<BuiltinType> BuiltinType::UnsignedInt =
		std::make_shared<BuiltinType>(u8"unsigned int");
	const std::shared_ptr<BuiltinType> BuiltinType::LongInt =
		std::make_shared<BuiltinType>(u8"long int");
	const std::shared_ptr<BuiltinType> BuiltinType::UnsignedLongInt =
		std::make_shared<BuiltinType>(u8"unsigned long int");
	const std::shared_ptr<BuiltinType> BuiltinType::LongLongInt =
		std::make_shared<BuiltinType>(u8"long long int");
	const std::shared_ptr<BuiltinType> BuiltinType::UnsignedLongLongInt =
		std::make_shared<BuiltinType>(u8"unsigned long long int");

	BuiltinType::BuiltinType(std::u8string_view name) noexcept
		: Name(name) {

		assert(!Name.empty());
	}

	JsonValue BuiltinType::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"BuiltinType").
			SetField(u8"name", std::u8string(Name)).
			Build();
	}

	bool BuiltinType::IsVoid() const noexcept {
		return Name == u8"void";
	}
	bool BuiltinType::IsUnsigned() const noexcept {
		return Name.front() == u8'u';
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

	JsonValue FunctionType::DumpJson() const {
		JsonArray parameterTypes;

		for (const auto& paramType : ParameterTypes) {
			parameterTypes.AddElement(paramType->DumpJson());
		}

		return JsonObject().
			SetField(u8"class", u8"FunctionType").
			SetField(u8"returnType", ReturnType->DumpJson()).
			SetField(u8"parameterTypes", parameterTypes.Build()).
			Build();
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