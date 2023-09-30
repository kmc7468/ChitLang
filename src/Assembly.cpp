#include <chit/Assembly.hpp>

#include <cassert>
#include <utility>

namespace chit {
	BodyStream& Assembly::AddFunction(
		std::u8string_view name,
		bool hasReturn,
		std::vector<std::u8string_view> parameters) {

		assert(!m_Functions.contains(name));

		auto& function = m_Functions[name];

		function.HasReturn = hasReturn;
		function.Parameters = std::move(parameters);

		return function.Body;
	}

	std::u8string Assembly::Generate() const {
		BodyStream stream;

		for (const auto& [name, function] : m_Functions) {
			if (function.HasReturn) {
				stream << u8"func ";
			} else {
				stream << u8"proc ";
			}

			stream << name << u8'(';

			bool isFirst = true;

			for (const auto& paramName : function.Parameters) {
				if (!isFirst) {
					stream << u8", ";
				} else {
					isFirst = false;
				}

				stream << paramName;
			}

			stream << u8"):\n"
				   << function.Body.view() << u8'\n';
		}

		return stream.str();
	}
}