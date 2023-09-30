#pragma once

#include <chit/Assembly.hpp>
#include <chit/Message.hpp>

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace chit {
	class Linker final {
	private:
		std::vector<const Assembly*> m_Assemblies;

		std::u8string m_ShitBF;
		std::vector<Message> m_Messages;

	public:
		Linker() noexcept = default;
		Linker(Linker&& other) noexcept = default;
		~Linker() = default;

	public:
		Linker& operator=(Linker&& other) noexcept = default;

	public:
		void AddAssembly(const Assembly* assembly) noexcept;

		void Link() noexcept;
		std::u8string_view GetShitBF() const noexcept;
		std::span<const Message> GetMessages() const noexcept;
	};
}