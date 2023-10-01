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

		Semicolon					= ';',
		Comma						= ',',

		LeftParenthesis				= '(',
		RightParenthesis			= ')',
		LeftBrace					= '{',
		RightBrace					= '}',

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