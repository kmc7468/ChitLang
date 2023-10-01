#pragma once

#include <cstddef>
#include <string_view>
#include <unordered_map>

namespace chit {
	enum class TokenType {
		None,
		Eof,

		Identifier,

		// Literals and Constants
		DecInteger,

		// Type keywords
		Void,
		Int,
		Long,
		Signed,
		Unsigned,

		// Other keywords,
		Return,
		If,
		Else,

		// Symbols
		Assignment,					// =
		Addition,					// +
		Subtraction,				// -
		Multiplication,				// *
		Division,					// /
		Modulo,						// %

		GreaterThan,				// >
		LessThan,					// <

		Semicolon,					// ;
		Comma,						// ,

		LeftParenthesis,			// (
		RightParenthesis,			// )
		LeftBrace,					// {
		RightBrace,					// }

		// Compound symbols
		Equivalence,				// ==
		NotEquivalence,				// !=
		GreaterThanOrEqual,			// >=
		LessThanOrEqual,			// <=
	};

	extern const std::unordered_map<
		TokenType,
		std::u8string_view> TokenSymbols;
	extern const std::unordered_map<
		std::u8string_view,
		TokenType> KeywordTokens;

	struct Token final {
		TokenType Type;
		std::u8string_view Data, Suffix;
		std::size_t Line, Column;
	};
}