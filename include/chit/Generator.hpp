#pragma once

#include <chit/Assembly.hpp>
#include <chit/Message.hpp>
#include <chit/ast/Node.hpp>

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace chit {
	class GeneratorContext final {
	public:
		GeneratorContext* const Parent = nullptr;

		chit::Assembly& Assembly;
		BodyStream* const Stream = nullptr;
		std::vector<Message>& Messages;

		std::unordered_set<std::u8string> TempIdentifiers;

	public:
		std::u8string_view CreateTempIdentifier();
		void DeleteTempIdentifier(std::u8string_view identifier);

	private:
		bool HasTempIdentifier(const std::u8string& identifier);
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