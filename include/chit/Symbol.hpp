#pragma once

#include <chit/Type.hpp>

#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace chit {
	enum class VariableState {
		Uninitialized,
		Unknown,
		Initalized,
	};

	struct VariableSymbol final {
		std::shared_ptr<Type> Type;
		VariableState State = VariableState::Uninitialized;
	};
}

namespace chit {
	struct FunctionSymbol final {
		std::shared_ptr<Type> ReturnType;
		std::vector<std::shared_ptr<Type>> ParameterTypes;
	};
}

namespace chit {
	using Symbol = std::variant<VariableSymbol, FunctionSymbol>;

	class SymbolTable final {
	private:
		SymbolTable* m_Parent = nullptr;

		std::unordered_map<
			std::u8string_view,
			std::unique_ptr<Symbol>> m_Symbols;

	public:
		SymbolTable() = default;
		explicit SymbolTable(SymbolTable* parent);
		SymbolTable(const SymbolTable&) = delete;
		~SymbolTable() = default;

	public:
		SymbolTable& operator=(const SymbolTable&) = delete;

	public:
		std::unique_ptr<SymbolTable> CreateChildTable();

		Symbol* CreateVariableSymbol(
			std::u8string_view name,
			std::shared_ptr<Type> type,
			VariableState state);
		Symbol* CreateFunctionSymbol(
			std::u8string_view name,
			std::shared_ptr<Type> returnType,
			std::vector<std::shared_ptr<Type>> parameterTypes);
		std::optional<std::pair<
			Symbol*,
			const SymbolTable*>> FindSymbol(std::u8string_view name);
	};
}