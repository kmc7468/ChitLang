#pragma once

#include <chit/Assembly.hpp>
#include <chit/Message.hpp>
#include <chit/ast/Node.hpp>

#include <optional>
#include <span>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace chit {
	class Context final {
	public:
		chit::Assembly& Assembly;
		std::vector<Message>& Messages;

		Context* Parent = nullptr;

		std::unordered_map<std::u8string_view, const Node*> Symbols;

	public:
		const Node* FindSymbol(std::u8string_view name) const noexcept;
	};
}

namespace chit {
	class Generator final {
	private:
		const RootNode* m_RootNode;

		std::optional<Assembly> m_Assembly;
		std::vector<Message> m_Messages;

	public:
		explicit Generator(const RootNode* rootNode) noexcept;
		Generator(Generator&& other) noexcept = default;
		~Generator() = default;

	public:
		Generator& operator=(Generator&& other) noexcept = default;

	public:
		void Generate();
		const Assembly* GetAssembly() const noexcept;
		std::span<const Message> GetMessages() const noexcept;
	};
}