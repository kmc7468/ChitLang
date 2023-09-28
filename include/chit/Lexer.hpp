#pragma once

#include <chit/Token.hpp>

#include <span>
#include <string>
#include <vector>

namespace chit {
	class Lexer final {
	private:
		std::string m_Source;
		std::size_t m_Cursor = 0;
		std::size_t m_Line = 1, m_Column = 1;

		std::vector<Token> m_Tokens;

	public:
		explicit Lexer(std::string source) noexcept;
		Lexer(Lexer&& other) noexcept = default;
		~Lexer() = default;

	public:
		Lexer& operator=(Lexer&& other) noexcept = default;

	public:
		void Lex();
		std::span<const Token> GetTokens() const noexcept;
	};
}