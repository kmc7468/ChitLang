#include <chit/Linker.hpp>

#include <cassert>

namespace chit {
	void Linker::AddAssembly(const Assembly* assembly) noexcept {
		m_Assemblies.push_back(assembly);
	}

	void Linker::Link() noexcept {
		assert(m_ShitBF.empty());

		for (const auto& assembly : m_Assemblies) {
			m_ShitBF.append(assembly->Generate());
		}

		m_ShitBF.append(
			u8"proc entrypoint:\n"
			u8"call main\n");
	}
	std::u8string_view Linker::GetShitBF() const noexcept {
		return m_ShitBF;
	}
	std::span<const Message> Linker::GetMessages() const noexcept {
		return m_Messages;
	}
}