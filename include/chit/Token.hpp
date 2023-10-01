#pragma once

#include <cstddef>
#include <string_view>

namespace chit {
	enum class TokenType {
		None,
		Eof,

		Identifier,

		// Symbols
		Assignment					= '=',
		Addition					= '+',
		Subtraction					= '-',
		Multiplication				= '*',
		Division					= '/',
		Modulo 						= '%',

		GreaterThan					= '>',
		LessThan					= '<',

		Semicolon					= ';',
		Comma						= ',',

		LeftParenthesis				= '(',
		RightParenthesis			= ')',
		LeftBrace					= '{',
		RightBrace					= '}',

		// Compound symbols
		Equivalence,				// ==
		NotEquivalence,				// !=
		GreaterThanOrEqual,			// >=
		LessThanOrEqual,			// <=

		// Type keywords
		Void,
		Int,
		Long,
		Signed,
		Unsigned,

		// Other keywords,
		Return,

		// Literals and Constants
		DecInteger,
	};

	struct Token final {
		TokenType Type;
		std::u8string_view Data, Suffix;
		std::size_t Line, Column;
	};
}