#include <chit/Token.hpp>

namespace chit {
	const std::unordered_map<
		TokenType,
		std::u8string_view> TokenSymbols{

		{ TokenType::Assignment, u8"=" },
		{ TokenType::Addition, u8"+" },
		{ TokenType::Subtraction, u8"-" },
		{ TokenType::Multiplication, u8"*" },
		{ TokenType::Division, u8"/" },
		{ TokenType::Modulo, u8"%" },

		{ TokenType::GreaterThan, u8">" },
		{ TokenType::LessThan, u8"<" },

		{ TokenType::Semicolon, u8";" },
		{ TokenType::Comma, u8"," },

		{ TokenType::LeftParenthesis, u8"(" },
		{ TokenType::RightParenthesis, u8")" },
		{ TokenType::LeftBrace, u8"{" },
		{ TokenType::RightBrace, u8"}" },

		{ TokenType::Equivalence, u8"==" },
		{ TokenType::NotEquivalence, u8"!=" },
		{ TokenType::GreaterThanOrEqual, u8">=" },
		{ TokenType::LessThanOrEqual, u8"<=" },
	};
}