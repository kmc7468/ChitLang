#include <chit/Generator.hpp>

#include <chit/ast/Node.hpp>

#include <cassert>

namespace chit {
	const Node* Context::FindSymbol(std::u8string_view name) const noexcept {
		if (const auto iterator = Symbols.find(name); iterator != Symbols.end()) {
			return iterator->second;
		} else if (Parent) {
			return Parent->FindSymbol(name);
		} else {
			return nullptr;
		}
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

		Context context{
			.Assembly = *m_Assembly,
			.Messages = m_Messages,
		};

		m_RootNode->Generate(context, nullptr);
	}
	const Assembly* Generator::GetAssembly() const noexcept {
		return &*m_Assembly;
	}
	std::span<const Message> Generator::GetMessages() const noexcept {
		return m_Messages;
	}
}