#pragma once

#include <chit/util/Json.hpp>

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

namespace chit {
	class GeneratorContext;

	class Type {
	public:
		Type() noexcept = default;
		Type(const Type&) = delete;
		virtual ~Type() = default;

	public:
		Type& operator=(const Type&) = delete;

	public:
		virtual JsonValue DumpJson() const = 0;
		virtual void GenerateConvert(GeneratorContext& context) const = 0;

		virtual bool IsEqual(const std::shared_ptr<Type>& other) const noexcept;
		virtual bool IsVoid() const noexcept;
	};

	using TypePtr = std::shared_ptr<Type>;
}

namespace chit {
	class BuiltinType final : public Type {
	public:
		static const std::shared_ptr<BuiltinType> Void;
		static const std::shared_ptr<BuiltinType> Int;
		static const std::shared_ptr<BuiltinType> UnsignedInt;
		static const std::shared_ptr<BuiltinType> LongInt;
		static const std::shared_ptr<BuiltinType> UnsignedLongInt;
		static const std::shared_ptr<BuiltinType> LongLongInt;
		static const std::shared_ptr<BuiltinType> UnsignedLongLongInt;

	public:
		std::u8string_view Name;
		std::optional<int> Rank;

	public:
		explicit BuiltinType(std::u8string_view name) noexcept;
		BuiltinType(std::u8string_view name, int rank) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void GenerateConvert(GeneratorContext& context) const override;

		virtual bool IsVoid() const noexcept override;
		bool IsUnsigned() const noexcept;

	public:
		static TypePtr RunUsualArithmeticConversion(
			TypePtr& newLeftType, const TypePtr& leftType,
			TypePtr& newRightType, const TypePtr& rightType);
		static void RunIntegerPromotion(TypePtr& newType, const TypePtr& type);
	};

	std::shared_ptr<BuiltinType> IsBuiltinType(const TypePtr& type) noexcept;
}

namespace chit {
	class FunctionType final : public Type {
	public:
		TypePtr ReturnType;
		std::vector<TypePtr> ParameterTypes;

	public:
		FunctionType(
			TypePtr returnType,
			std::vector<TypePtr> parameterTypes) noexcept;

	public:
		virtual JsonValue DumpJson() const override;
		virtual void GenerateConvert(GeneratorContext& context) const override;

		virtual bool IsEqual(const TypePtr& other) const noexcept override;
	};

	std::shared_ptr<FunctionType> IsFunctionType(const TypePtr& type) noexcept;
}