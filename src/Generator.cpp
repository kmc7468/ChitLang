#include <chit/Generator.hpp>

#include <chit/ast/Node.hpp>

#include <cassert>
#include <cstdint>
#include <random>

namespace chit {
	std::u8string_view GeneratorContext::CreateTempIdentifier() {
		if (Parent && Parent->Stream == Stream)
			return Parent->CreateTempIdentifier();

		while (true) {
			thread_local std::mt19937 mt(std::random_device{}());
			thread_local std::uniform_int_distribution<std::uint16_t> dist(0, 0xFFFF);

			const std::string randomName = "_ChitLangTemp" + std::to_string(dist(mt));
			const std::u8string utf8RandomName(randomName.begin(), randomName.end());

			if (!HasTempIdentifier(utf8RandomName)) {
				return *TempIdentifiers.insert(std::move(utf8RandomName)).first;
			}
		}
	}
	void GeneratorContext::DeleteTempIdentifier(std::u8string_view identifier) {
		if (Parent && Parent->Stream == Stream) {
			Parent->DeleteTempIdentifier(identifier);
			return;
		}

		assert(TempIdentifiers.contains(std::u8string(identifier)));

		TempIdentifiers.erase(std::u8string(identifier));
	}

	bool GeneratorContext::HasTempIdentifier(const std::u8string& identifier) {
		return
			TempIdentifiers.contains(identifier) ||
			(Parent && Parent->HasTempIdentifier(identifier));
	}
}

namespace chit {
	Generator::Generator(const RootNode* rootNode) noexcept
		: m_RootNode(rootNode) {

		assert(m_RootNode != nullptr);
	}

	void Generator::Generate() {
		assert(m_Assembly == std::nullopt);

		m_Assembly = Assembly{};

		GeneratorContext context{
			.Assembly = *m_Assembly,
			.Messages = m_Messages,
		};

		m_RootNode->Generate(context);
	}
	const Assembly* Generator::GetAssembly() const noexcept {
		return &*m_Assembly;
	}
	std::span<const Message> Generator::GetMessages() const noexcept {
		return m_Messages;
	}
}