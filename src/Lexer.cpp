#include <chit/Lexer.hpp>

#include <chit/util/Unicode.hpp>

#include <cassert>
#include <cctype>
#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <utf8.h>
#include <utility>

namespace chit {
	Lexer::Lexer(std::u8string source) noexcept
		: m_Source(std::move(source)) {
		m_Current = m_Source.begin();
	}

	void Lexer::Lex() {
		assert(m_Current == m_Source.begin());
		assert(m_Tokens.empty());

		if (m_Source.back() != u8'\n') {
			m_Source.push_back(u8'\n');
			m_Current = m_Source.begin();
		}

		while (m_Current < m_Source.end()) {
			const auto cursor = NextCursor();
			const auto& codepoint = cursor.Codepoint;

			if (codepoint == U'\n') {
				++m_Line;
				m_Column = 0;

				continue;
			} else if (std::isspace(codepoint)) {
				continue;
			}

			if (std::isdigit(codepoint)) {
				LexNumber(cursor);
			} else if (IsSpecialSymbol(codepoint)) {
				LexSpeicalSymbol(cursor);
			} else {
				LexIdentifier(cursor);
			}
		}

		m_Tokens.push_back({
			.Type = TokenType::Eof,
			.Line = m_Line,
			.Column = m_Column,
		});
	}
	std::span<const Token> Lexer::GetTokens() const noexcept {
		return m_Tokens;
	}
	std::span<const Message> Lexer::GetMessages() const noexcept {
		return m_Messages;
	}

	Lexer::Cursor Lexer::NextCursor() {
		const std::u8string::iterator iterator = m_Current;
		const char32_t codepoint = utf8::unchecked::next(m_Current);

		return {
			.Iterator = iterator,
			.Column = ++m_Column,
			.Codepoint = codepoint,
		};
	}
	void Lexer::PrevCursor() {
		utf8::unchecked::prior(m_Current);
		--m_Column;
	}

	void Lexer::LexNumber(const Cursor& begin) {
		while (m_Current < m_Source.end()) {
			const auto next = NextCursor();
			const auto& codepoint = next.Codepoint;

			if (!std::isdigit(codepoint)) {
				m_Tokens.push_back({
					.Type = TokenType::DecInteger,
					.Data = { begin.Iterator, next.Iterator },
					.Line = m_Line,
					.Column = begin.Column,
				});

				PrevCursor();
				break;
			}
		}
	}
	void Lexer::LexSpeicalSymbol(const Cursor& begin) {
#define CASE(c, e)													\
		case c:														\
			m_Tokens.push_back({									\
				.Type = TokenType::e,								\
				.Data = { begin.Iterator, begin.Iterator + 1 },		\
				.Line = m_Line,										\
				.Column = begin.Column,								\
			});														\
																	\
			break

		switch (begin.Codepoint) {
		CASE(U'=', Assignment);

		CASE(U';', Semicolon);

		CASE(U'(', LeftParenthesis);
		CASE(U')', RightParenthesis);
		CASE(U'{', LeftBrace);
		CASE(U'}', RightBrace);

		default:
			m_Messages.push_back({
				.Type = MessageType::Error,
				.Data = u8"Unrecognized token",
				.Line = m_Line,
				.Column = begin.Column,
			});

			break;
		}
	}
	void Lexer::LexIdentifier(const Cursor& begin) {
		while (m_Current < m_Source.end()) {
			const auto next = NextCursor();
			const auto& codepoint = next.Codepoint;

			if (std::isspace(codepoint) || IsSpecialSymbol(codepoint) && codepoint != U'_') {
				Token token{
					.Type = TokenType::Identifier,
					.Data = { begin.Iterator, next.Iterator },
					.Line = m_Line,
					.Column = begin.Column,
				};

				static const std::unordered_map<std::u8string_view, TokenType> keywords{
					{ u8"void", TokenType::Void },
					{ u8"int", TokenType::Int },
				};
				if (const auto keywordIter = keywords.find(token.Data);
					keywordIter != keywords.end()) {

					token.Type = keywordIter->second;
				}
				
				m_Tokens.push_back(std::move(token));

				PrevCursor();
				break;
			}
		}
	}
}