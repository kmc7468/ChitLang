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
}