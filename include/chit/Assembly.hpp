#pragma once

#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace chit {
	using BodyStream = std::basic_stringstream<char8_t>;

	class Assembly final {
	private:
		struct Function final {
			bool HasReturn;
			std::vector<std::u8string_view> Parameters;
			BodyStream Body;
		};

	private:
		std::unordered_map<std::u8string_view, Function> m_Functions;

	public:
		Assembly() noexcept = default;
		Assembly(Assembly&& other) noexcept = default;
		~Assembly() = default;

	public:
		Assembly& operator=(Assembly&& other) noexcept = default;

	public:
		BodyStream& AddFunction(
			std::u8string_view name,
			bool hasReturn,
			std::vector<std::u8string_view> parameters);

		std::u8string Generate() const;
	};
}