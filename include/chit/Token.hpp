#pragma once

#include <cstddef>
#include <string_view>

namespace chit {
	enum class TokenType {
		None,
		Eof,

		Identifier,

		// Type Keywords
		Void,
		Int,

		// Literals and Constants
		DecInteger,

		// Symbols
		Assignment,							// =

		Semicolon,							// ;
		Comma,								// ,

		LeftParenthesis,					// (
		RightParenthesis,					// )
		LeftBrace,							// {
		RightBrace,							// }
	};

	struct Token final {
		TokenType Type;
		std::u8string_view Data;
		std::size_t Line, Column;
	};
}