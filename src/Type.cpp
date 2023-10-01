#include <chit/Type.hpp>

#include <chit/Generator.hpp>

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
		std::make_shared<BuiltinType>(u8"int", 0);
	const std::shared_ptr<BuiltinType> BuiltinType::UnsignedInt =
		std::make_shared<BuiltinType>(u8"unsigned int", 0);
	const std::shared_ptr<BuiltinType> BuiltinType::LongInt =
		std::make_shared<BuiltinType>(u8"long int", 1);
	const std::shared_ptr<BuiltinType> BuiltinType::UnsignedLongInt =
		std::make_shared<BuiltinType>(u8"unsigned long int", 1);
	const std::shared_ptr<BuiltinType> BuiltinType::LongLongInt =
		std::make_shared<BuiltinType>(u8"long long int", 2);
	const std::shared_ptr<BuiltinType> BuiltinType::UnsignedLongLongInt =
		std::make_shared<BuiltinType>(u8"unsigned long long int", 2);

	BuiltinType::BuiltinType(std::u8string_view name) noexcept
		: Name(name) {

		assert(!Name.empty());
	}
	BuiltinType::BuiltinType(std::u8string_view name, int rank) noexcept
		: Name(name), Rank(rank) {

		assert(!Name.empty());
	}

	JsonValue BuiltinType::DumpJson() const {
		return JsonObject().
			SetField(u8"class", u8"BuiltinType").
			SetField(u8"name", std::u8string(Name)).
			SetField(u8"rank", Rank ? *Rank : JsonNull()).
			Build();
	}
	void BuiltinType::GenerateConvert(GeneratorContext& context) const {
		assert(!IsVoid());
		assert(Rank);
		assert(context.Stream);

		switch (*Rank) {
		case 0:
		case 1:
			*context.Stream << u8"toi\n";

			break;

		case 2:
			*context.Stream << u8"tol\n";

			break;

		default:
			assert(false);
		}
	}

	bool BuiltinType::IsVoid() const noexcept {
		return this == BuiltinType::Void.get();
	}
	bool BuiltinType::IsUnsigned() const noexcept {
		return Name.front() == u8'u';
	}

	TypePtr BuiltinType::RunUsualArithmeticConversion(
		TypePtr& newLeftType, const TypePtr& leftType,
		TypePtr& newRightType, const TypePtr& rightType) {

		if (leftType == rightType)
			return leftType;

		const auto builtinLeftType =
			std::dynamic_pointer_cast<BuiltinType>(leftType);
		const auto builtinRightType =
			std::dynamic_pointer_cast<BuiltinType>(rightType);

		assert(builtinLeftType);
		assert(builtinRightType);

		if (builtinLeftType->IsUnsigned() == builtinRightType->IsUnsigned()) {
			if (builtinLeftType->Rank < builtinRightType->Rank) {
				newLeftType = rightType;

				return newLeftType;
			} else {
				newRightType = leftType;

				return newRightType;
			}
		}

		auto& newUnsignedType =
			builtinLeftType->IsUnsigned() ? newLeftType : newRightType;
		auto& newSignedType =
			builtinLeftType->IsUnsigned() ? newRightType : newLeftType;
		const auto& unsignedType =
			builtinLeftType->IsUnsigned() ? builtinLeftType : builtinRightType;
		const auto& signedType =
			builtinLeftType->IsUnsigned() ? builtinRightType : builtinLeftType;

		if (unsignedType->Rank >= signedType->Rank) {
			newSignedType = unsignedType;

			return newSignedType;
		} else {
			newUnsignedType = signedType;

			return newUnsignedType;
		}
	}
	void BuiltinType::RunIntegerPromotion(TypePtr&, const TypePtr&) {
		// TODO
	}

	std::shared_ptr<BuiltinType> IsBuiltinType(const TypePtr& type) noexcept {
		return std::dynamic_pointer_cast<BuiltinType>(type);
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
	void FunctionType::GenerateConvert(GeneratorContext&) const {
		assert(false);
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

	std::shared_ptr<FunctionType> IsFunctionType(const TypePtr& type) noexcept {
		return std::dynamic_pointer_cast<FunctionType>(type);
	}
}