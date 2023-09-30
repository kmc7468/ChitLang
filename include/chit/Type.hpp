#pragma once

#include <chit/Assembly.hpp>
#include <chit/util/Json.hpp>

#include <memory>
#include <string_view>
#include <vector>

namespace chit {
	class Type {
	public:
		Type() noexcept = default;
		Type(const Type&) = delete;
		virtual ~Type() = default;

	public:
		Type& operator=(const Type&) = delete;

	public:
		virtual JsonValue DumpJson() const = 0;

		virtual bool IsEqual(
			const std::shared_ptr<Type>& other) const noexcept;
		virtual bool IsVoid() const noexcept;
	};

	using TypePtr = std::shared_ptr<Type>;
}

namespace chit {
	class BuiltinType final : public Type {
	public:
		static const std::shared_ptr<BuiltinType> Void;
		static const std::shared_ptr<BuiltinType> Int;

	public:
		std::u8string_view Name;

	public:
		explicit BuiltinType(std::u8string_view name) noexcept;

	public:
		virtual JsonValue DumpJson() const override;

		virtual bool IsVoid() const noexcept override;
	};
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

		virtual bool IsEqual(const TypePtr& other) const noexcept override;
	};;
}