#pragma once

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <variant>
#include <vector>
#include <utility>

namespace chit {
	class JsonArray;
	class JsonObject;

	using JsonValue = std::variant<
		std::monostate,					// Null
		std::u8string,					// String
		std::int64_t,					// Signed integer
		bool,							// Boolean
		std::unique_ptr<JsonArray>,		// Array
		std::unique_ptr<JsonObject>>;	// Object

	JsonValue JsonNull() noexcept;

	std::basic_ostream<char8_t>& operator<<(
		std::basic_ostream<char8_t>& stream,
		const JsonValue& value);
}

namespace chit {
	class JsonArray final {
		friend std::basic_ostream<char8_t>& operator<<(
			std::basic_ostream<char8_t>& stream,
			const JsonArray& array);

	private:
		std::vector<JsonValue> m_Elements;

	public:
		JsonArray() noexcept = default;
		explicit JsonArray(std::vector<JsonValue> elements) noexcept;
		JsonArray(JsonArray&& other) noexcept = default;
		~JsonArray() = default;

	public:
		JsonArray& operator=(JsonArray&& other) noexcept = default;

	public:
		JsonArray& AddElement(JsonValue value);
		JsonValue Build();
	};

	std::basic_ostream<char8_t>& operator<<(
		std::basic_ostream<char8_t>& stream,
		const JsonArray& array);
}

namespace chit {
	class JsonObject final {
		friend std::basic_ostream<char8_t>& operator<<(
			std::basic_ostream<char8_t>& stream,
			const JsonObject& object);

	private:
		std::vector<std::pair<std::u8string, JsonValue>> m_Fields;

	public:
		JsonObject() noexcept = default;
		JsonObject(JsonObject&& other) noexcept = default;
		~JsonObject() = default;

	public:
		JsonObject& operator=(JsonObject&& other) noexcept = default;

	public:
		JsonObject& SetField(std::u8string name, JsonValue value);
		JsonObject& Merge(JsonValue&& object);
		JsonValue Build();
	};

	std::basic_ostream<char8_t>& operator<<(
		std::basic_ostream<char8_t>& stream,
		const JsonObject& object);
}