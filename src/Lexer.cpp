#include <chit/Lexer.hpp>

#include <utility>

namespace chit {
	Lexer::Lexer(std::u8string source) noexcept
		: m_Source(std::move(source)) {}

	void Lexer::Lex() {
		// TODO
	}
	std::span<const Token> Lexer::GetTokens() const noexcept {
		return m_Tokens;
	}
	std::span<const Message> Lexer::GetMessages() const noexcept {
		return m_Messages;
	}
}