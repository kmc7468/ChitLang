#include <chit/Symbol.hpp>

namespace chit {
	SymbolTable::SymbolTable(SymbolTable* parent)
		: m_Parent(parent) {}

	std::unique_ptr<SymbolTable> SymbolTable::CreateChildTable() {
		return std::make_unique<SymbolTable>(this);
	}

	Symbol* SymbolTable::CreateVariableSymbol(
		std::u8string_view name,
		std::shared_ptr<Type> type,
		VariableState state) {

		auto& symbol = m_Symbols[name];

		symbol = std::unique_ptr<Symbol>(new Symbol(VariableSymbol{
			.Type = std::move(type),
			.State = state,
		}));

		return symbol.get();
	}
	Symbol* SymbolTable::CreateFunctionSymbol(
		std::u8string_view name,
		std::shared_ptr<Type> returnType,
		std::vector<std::shared_ptr<Type>> parameterTypes) {

		auto& symbol = m_Symbols[name];

		symbol = std::unique_ptr<Symbol>(new Symbol(FunctionSymbol{
			.ReturnType = std::move(returnType),
			.ParameterTypes = std::move(parameterTypes),
		}));

		return symbol.get();
	}
	std::optional<std::pair<
		Symbol*,
		const SymbolTable*>> SymbolTable::FindSymbol(std::u8string_view name) {

		if (const auto symIter = m_Symbols.find(name);
			symIter != m_Symbols.end()) {

			return std::make_pair(symIter->second.get(), this);
		} else if (m_Parent) {
			return m_Parent->FindSymbol(name);
		} else {
			return std::nullopt;
		}
	}
}