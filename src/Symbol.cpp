#include <chit/Symbol.hpp>

namespace chit {
	VariableSymbol* IsVariableSymbol(Symbol* symbol) noexcept {
		if (!symbol) return nullptr;

		if (std::holds_alternative<VariableSymbol>(*symbol)) {
			return &std::get<VariableSymbol>(*symbol);
		} else {
			return nullptr;
		}
	}
	FunctionSymbol* IsFunctionSymbol(Symbol* symbol) noexcept {
		if (!symbol) return nullptr;

		if (std::holds_alternative<FunctionSymbol>(*symbol)) {
			return &std::get<FunctionSymbol>(*symbol);
		} else {
			return nullptr;
		}
	}
}

namespace chit {
	SymbolTable::SymbolTable(SymbolTable& parent)
		: m_Parent(&parent) {}

	Symbol* SymbolTable::CreateVariableSymbol(
		std::u8string_view name,
		TypePtr type,
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
		TypePtr returnType,
		std::vector<TypePtr> parameterTypes) {

		auto& symbol = m_Symbols[name];

		symbol = std::unique_ptr<Symbol>(new Symbol(FunctionSymbol{
			.Type = std::shared_ptr<FunctionType>(new FunctionType(
				std::move(returnType),
				std::move(parameterTypes))),
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

	bool SymbolTable::IsGlobal() const noexcept {
		return m_Parent == nullptr;
	}
}