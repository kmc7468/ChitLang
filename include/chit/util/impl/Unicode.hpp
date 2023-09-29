#pragma once
#include "../Unicode.hpp"

namespace chit {
	constexpr int IsSpecialSymbol(char32_t codepoint) noexcept {
		switch (codepoint) {
		case '~':
		case '`':
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '^':
		case '&':
		case '*':
		case '(':
		case ')':
		case '_':
		case '-':
		case '+':
		case '=':
		case '{':
		case '[':
		case '}':
		case ']':
		case '|':
		case '\\':
		case ':':
		case ';':
		case '"':
		case '\'':
		case '<':
		case ',':
		case '>':
		case '.':
		case '?':
		case '/':
			return true;

		default:
			return false;
		}
	}
}