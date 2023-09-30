#include <chit/util/Json.hpp>

#include <chit/util/String.hpp>

#include <algorithm>
#include <cassert>

namespace chit {
	JsonValue JsonNull() noexcept {
		return std::monostate{};
	}

	std::basic_ostream<char8_t>& operator<<(
		std::basic_ostream<char8_t>& stream,
		const JsonValue& value) {

		if (std::holds_alternative<std::monostate>(value)) {
			return stream << u8"null";
		} else if (std::holds_alternative<std::u8string>(value)) {
			return stream << u8'"' << std::get<std::u8string>(value) << u8'"';
		} else if (std::holds_alternative<long long>(value)) {
			return stream << ToUtf8String(std::get<long long>(value));
		} else if (std::holds_alternative<bool>(value)) {
			return stream << (std::get<bool>(value) ? u8"true" : u8"false");
		} else if (std::holds_alternative<std::unique_ptr<JsonArray>>(value)) {
			return stream << *std::get<std::unique_ptr<JsonArray>>(value);
		} else {
			return stream << *std::get<std::unique_ptr<JsonObject>>(value);
		}
	}
}

namespace chit {
	JsonArray::JsonArray(std::vector<JsonValue> elements) noexcept
		: m_Elements(std::move(elements)) {}

	JsonArray& JsonArray::AddElement(JsonValue value) {
		m_Elements.push_back(std::move(value));

		return *this;
	}
	JsonValue JsonArray::Build() {
		return std::unique_ptr<JsonArray>(new JsonArray(std::move(*this)));
	}

	std::basic_ostream<char8_t>& operator<<(
		std::basic_ostream<char8_t>& stream,
		const JsonArray& array) {

		stream << u8'[';

		bool isFirst = true;

		for (const auto& element : array.m_Elements) {
			if (!isFirst) {
				stream << u8',';
			} else {
				isFirst = false;
			}

			stream << element;
		}

		return stream << u8']';
	}
}

namespace chit {
	JsonObject& JsonObject::SetField(std::u8string name, JsonValue value) {
		assert(!name.empty());
		
		if (const auto fieldIter =
				std::find_if(m_Fields.begin(), m_Fields.end(), [&name](const auto& field) {
					return field.first == name;
				});
			fieldIter != m_Fields.end()) {

			fieldIter->second = std::move(value);
		} else {
			m_Fields.push_back({ std::move(name), std::move(value) });
		}

		return *this;
	}
	JsonObject& JsonObject::Merge(JsonValue&& value) {
		assert(std::holds_alternative<std::unique_ptr<JsonObject>>(value));

		auto& valueObject = *std::get<std::unique_ptr<JsonObject>>(value);

		for (auto& [name, value] : valueObject.m_Fields) {
			SetField(std::move(name), std::move(value));
		}

		return *this;
	}
	JsonValue JsonObject::Build() {
		return std::unique_ptr<JsonObject>(new JsonObject(std::move(*this)));
	}

	std::basic_ostream<char8_t>& operator<<(
		std::basic_ostream<char8_t>& stream,
		const JsonObject& object) {

		stream << u8'{';

		bool isFirst = true;

		for (const auto& [name, value] : object.m_Fields) {
			if (!isFirst) {
				stream << u8',';
			} else {
				isFirst = false;
			}

			stream << u8'"' << name << u8"\":";
			stream << value;
		}

		return stream << u8'}';
	}
}